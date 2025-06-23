#include "../../include/core/Scheduler.h"
#include "../../include/core/ConfigData.h"
#include "../../include/domains/Warehouse.h"
#include "../../include/utils/Logger.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

Scheduler::Scheduler(const ConfigData& configData, int maxEvents) : eventsHeap(maxEvents), timer() {
    if (configData.packages == nullptr || configData.warehouses == nullptr || configData.graph == nullptr) {
        throw std::invalid_argument("Dados de configuração inválidos foram passados para o Scheduler.");
    }
    //* Inicializa os atributos do Scheduler com os dados de configuração
    this->transportCapacity = configData.transportCapacity;
    this->transportLatency = configData.transportLatency;
    this->transportInterval = configData.transportInterval;
    this->removalCost = configData.removalCost;

    this->activePackages = configData.numPackages;

    //* Insere os eventos de chegada de pacotes no heap de eventos
    for (int i = 0; i < configData.numPackages; ++i) {
        Package* package = configData.packages[i];
        if (package != nullptr) {
            this->eventsHeap.insert(new Event(package->getPostTime(), package));
        }
    }
}

Scheduler::~Scheduler() {
    //* Limpa todos os eventos restantes no heap
    while (!this->eventsHeap.isEmpty()) {
        Event* event = this->eventsHeap.extractMin();
        delete event;
    }
}

void Scheduler::runSimulation(Warehouse** warehouses, Graph* graph, int numWarehouses) {
    if (this->activePackages > 0) {
        initializeTransportEvents(graph, numWarehouses);
    }

    //* Enquanto houver eventos no heap, processa cada evento
    while (!this->eventsHeap.isEmpty()) {
        Event* event = this->removeNextEvent();
        if (event == nullptr)
            continue;

        //* Se o tempo do evento for menor que o tempo atual do timer, ignora o evento
        //* Isso garante que os eventos sejam processados na ordem correta
        //* e evita que eventos com tempos passados sejam processados novamente
        if (event->time < this->timer.getTime())
            continue;

        //* Avança o timer para o tempo do evento atual
        this->timer.setTime(event->time);

        //* Dependendo do tipo de evento, chama a função apropriada para processá-lo
        switch (event->type) {
        case EventType::PACKAGE_ARRIVAL:
            handlePackageArrival(event, warehouses);
            break;
        case EventType::LEAVE_TRANSPORT:
            handleTransportDeparture(event, warehouses);
            break;
        default:
            break;
        }

        //* Deleta o evento após processá-lo
        delete event;
    }
}

void Scheduler::initializeTransportEvents(Graph* graph, int numWarehouses) {
    if (this->eventsHeap.isEmpty())
        return;

    //* Inicializa os eventos de transporte com base no grafo e no tempo do primeiro pacote
    //* O tempo do primeiro pacote é usado como base para calcular o tempo inicial de transporte
    double firstPackageTime = this->eventsHeap.peekMin()->time;
    double initialTransportTime = firstPackageTime + this->transportInterval;

    //* Insere eventos de transporte para cada par de armazéns conectados no grafo
    for (int i = 0; i < numWarehouses; ++i) {
        for (int j = 0; j < numWarehouses; ++j) {
            if (graph->hasEdge(i, j)) {
                this->eventsHeap.insert(new Event(initialTransportTime, i, j));
            }
        }
    }
}

void Scheduler::handlePackageArrival(Event* event, Warehouse** warehouses) {
    //* Verifica se o evento é válido e se o pacote está presente
    Package* package = event->package;
    if (!package)
        return;
    int currentWarehouseId = package->getRoute().peekFront();

    //* Se a rota do pacote so tem um armazém, significa que o pacote chegou ao destino final
    if (package->getRoute().getCurrentSize() == 1) {
        //* Se o pacote chegou ao destino final, atualiza o estado para DELIVERED
        package->setState(PackageState::DELIVERED);
        logPackageDelivered(timer.getTime(), package->getId(), currentWarehouseId);
        this->activePackages--;
    } else {
        //* Se o pacote ainda tem armazéns na rota, remove o armazém atual da rota
        //* e armazena o pacote no próximo armazém da rota
        package->getRoute().removeFront();
        int nextWarehouseId = package->getRoute().peekFront();
        warehouses[currentWarehouseId]->storePackage(package, nextWarehouseId);
        package->setCurrentLocation(currentWarehouseId);
        package->setState(PackageState::STORED);
        logPackageStored(timer.getTime(), package->getId(), currentWarehouseId, nextWarehouseId);
    }
}

void Scheduler::handleTransportDeparture(Event* event, Warehouse** warehouses) {
    //* Verifica se o evento é válido e se os armazéns de origem e destino estão presentes
    int originId = event->originWarehouseId;
    int sectionId = event->destinationSectionId;
    Warehouse* originWarehouse = warehouses[originId];

    //* Se ainda houver pacotes ativos, insere um novo evento de transporte no heap
    if (this->activePackages > 0) {
        int nextTransportTime = event->time + this->transportInterval;
        this->eventsHeap.insert(new Event(nextTransportTime, originId, sectionId));
    }

    //* Se a seção do armazém de origem estiver vazia, não há pacotes para transportar
    if (originWarehouse->isSectionEmpty(sectionId)) {
        return;
    }

    //* Recupera os pacotes da seção do armazém de origem usando uma pilha LIFO
    //* Isso garante que os pacotes sejam removidos na ordem inversa em que foram armazenados
    int numPackagesInSection = originWarehouse->getSectionSize(sectionId);
    Package** lifoBuffer = new Package*[numPackagesInSection];
    for (int i = 0; i < numPackagesInSection; ++i) {
        lifoBuffer[i] = originWarehouse->retrievePackage(sectionId);
    }

    //* Registra o tempo de remoção dos pacotes
    double lastRemovalTime = event->time;
    //* Registra a remoção de cada pacote da seção do armazém de origem
    for (int i = 0; i < numPackagesInSection; i++) {
        lastRemovalTime += this->removalCost;
        logPackageRemoved(lastRemovalTime, lifoBuffer[i]->getId(), originId, sectionId);
    }

    //* Calcula quantos pacotes serão transportados e quantos serão restaurados
    int numToTransport = std::min(numPackagesInSection, this->transportCapacity);
    int numToRestore = numPackagesInSection - numToTransport;

    //* Registra os pacotes que estão sendo transportados e restaura os pacotes restantes
    for (int i = numPackagesInSection - 1; i >= numToRestore; --i) {
        Package* package = lifoBuffer[i];
        logPackageInTransit(lastRemovalTime, package->getId(), originId, sectionId);

        int arrivalTime = lastRemovalTime + this->transportLatency;
        this->eventsHeap.insert(new Event(arrivalTime, package));
    }

    //* Restaura os pacotes restantes na seção do armazém de origem
    //* Isso garante que os pacotes que não foram transportados ainda estejam disponíveis
    for (int i = numToRestore - 1; i >= 0; --i) {
        Package* package = lifoBuffer[i];
        originWarehouse->storePackage(package, sectionId);
        logPackageRestored(lastRemovalTime, package->getId(), originId, sectionId);
    }

    //* Libera a memória alocada para o buffer LIFO
    delete[] lifoBuffer;
}

Event* Scheduler::getNextEvent() {
    return this->eventsHeap.peekMin();
}

Event* Scheduler::removeNextEvent() {
    return this->eventsHeap.extractMin();
}

Timer::Timer() : time(0) {}

int Timer::getTime() const noexcept {
    return this->time;
}

void Timer::setTime(int newTime) {
    this->time = newTime;
}

void Timer::addTime(int time) {
    if (time < 0) {
        throw std::invalid_argument("Time cannot be negative");
    }
    this->time += time;
}
#include "../../include/core/Scheduler.h"
#include "../../include/ui/OutputLogger.h"
#include <algorithm> // Necessário para std::min
#include <iostream>
#include <stdexcept>

// O construtor, destrutor e outras funções permanecem os mesmos.
// ... (seu construtor, destrutor, runSimulation, etc.)

Scheduler::Scheduler(const IO::ConfigData& configData, int maxEvents) : eventsHeap(maxEvents), timer() {
    if (configData.packages == nullptr || configData.warehouses == nullptr || configData.graph == nullptr) {
        throw std::invalid_argument("Dados de configuração inválidos foram passados para o Scheduler.");
    }
    this->transportCapacity = configData.transportCapacity;
    this->transportLatency = configData.transportLatency;
    this->transportInterval = configData.transportInterval;
    this->removalCost = configData.removalCost;

    this->activePackages = configData.numPackages;

    for (int i = 0; i < configData.numPackages; ++i) {
        Package* package = configData.packages[i];
        if (package != nullptr) {
            this->eventsHeap.insert(new Event(package->getPostTime(), package));
        }
    }
}

Scheduler::~Scheduler() {
    while (!this->eventsHeap.isEmpty()) {
        Event* ev = this->eventsHeap.extractMin();
        if (ev)
            delete ev;
    }
}

void Scheduler::runSimulation(Warehouse** warehouses, Graph* graph, int numWarehouses) {
    if (this->activePackages > 0) {
        initializeTransportEvents(graph, numWarehouses);
    }

    while (!this->eventsHeap.isEmpty()) {
        Event* event = this->removeNextEvent();
        if (event == nullptr)
            continue;

        if (event->time < this->timer.getTime())
            continue; // Ignora eventos passados

        this->timer.setTime(event->time);
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
        delete event;
    }
}

void Scheduler::initializeTransportEvents(Graph* graph, int numWarehouses) {
    // Para simplificar e garantir o primeiro transporte, vamos encontrar o tempo do primeiro pacote
    // e agendar o transporte inicial a partir dali, como no código de referência.
    // Se não houver pacotes, nenhum evento de transporte será criado aqui.
    if (this->eventsHeap.isEmpty())
        return;

    double firstPackageTime = this->eventsHeap.peekMin()->time;
    double initialTransportTime = firstPackageTime + this->transportInterval;

    for (int i = 0; i < numWarehouses; ++i) {
        for (int j = 0; j < numWarehouses; ++j) {
            if (graph->hasEdge(i, j)) {
                this->eventsHeap.insert(new Event(initialTransportTime, i, j));
            }
        }
    }
}

void Scheduler::handlePackageArrival(Event* event, Warehouse** warehouses) {
    Package* package = event->package;
    if (!package)
        return;
    int currentWarehouseId = package->getRoute().peekFront();

    if (package->getRoute().getCurrentSize() == 1) {
        package->setState(PackageState::DELIVERED);
        OutputLogger::logPackageDelivered(timer.getTime(), package->getId(), currentWarehouseId);
        this->activePackages--;
    } else {
        package->getRoute().removeFront();
        int nextWarehouseId = package->getRoute().peekFront();
        warehouses[currentWarehouseId]->storePackage(package, nextWarehouseId);
        package->setCurrentLocation(currentWarehouseId);
        package->setState(PackageState::STORED);
        OutputLogger::logPackageStored(timer.getTime(), package->getId(), currentWarehouseId, nextWarehouseId);
    }
}

// =================================================================================
// FUNÇÃO CORRIGIDA E FINAL: handleTransportDeparture
// =================================================================================
void Scheduler::handleTransportDeparture(Event* event, Warehouse** warehouses) {
    int originId = event->originWarehouseId;
    int sectionId = event->destinationSectionId;
    Warehouse* originWarehouse = warehouses[originId];

    // 1. Agenda o próximo transporte para esta rota, se ainda houver pacotes ativos.
    if (this->activePackages > 0) {
        int nextTransportTime = event->time + this->transportInterval;
        this->eventsHeap.insert(new Event(nextTransportTime, originId, sectionId));
    }

    if (originWarehouse->isSectionEmpty(sectionId)) {
        return; // Nada a fazer se a seção já está vazia.
    }

    // 2. Remove todos os pacotes da seção (LIFO) e armazena-os temporariamente.
    int numPackagesInSection = originWarehouse->getSectionSize(sectionId);
    Package** lifoBuffer = new Package*[numPackagesInSection];
    for (int i = 0; i < numPackagesInSection; ++i) {
        lifoBuffer[i] = originWarehouse->retrievePackage(sectionId);
    }

    // 3. Loga a remoção de cada pacote com seu custo de tempo individual.
    double lastRemovalTime = event->time;
    for (int i = 0; i < numPackagesInSection; i++) {
        lastRemovalTime += this->removalCost;
        OutputLogger::logPackageRemoved(lastRemovalTime, lifoBuffer[i]->getId(), originId, sectionId);
    }

    // 4. Determina quais pacotes serão transportados e quais serão re-armazenados.
    // A lógica é LIFO: os últimos a serem removidos (topo da pilha) são os primeiros a serem transportados.
    int numToTransport = std::min(numPackagesInSection, this->transportCapacity);
    int numToRestore = numPackagesInSection - numToTransport;

    // 5. Loga o trânsito e agenda a chegada dos pacotes transportados.
    // O loop vai dos pacotes que foram removidos por último para os primeiros.
    for (int i = numPackagesInSection - 1; i >= numToRestore; --i) {
        Package* package = lifoBuffer[i];
        OutputLogger::logPackageInTransit(lastRemovalTime, package->getId(), originId, sectionId);

        int arrivalTime = lastRemovalTime + this->transportLatency;
        this->eventsHeap.insert(new Event(arrivalTime, package));
    }

    // 6. Re-armazena os pacotes restantes na ordem inversa da remoção.
    // O loop vai dos pacotes que foram removidos primeiro até o limite dos que não foram transportados.
    for (int i = numToRestore - 1; i >= 0; --i) {
        Package* package = lifoBuffer[i];
        originWarehouse->storePackage(package, sectionId);
        OutputLogger::logPackageRestored(lastRemovalTime, package->getId(), originId, sectionId);
    }

    // 7. Libera a memória alocada para o buffer.
    delete[] lifoBuffer;
}

Event* Scheduler::getNextEvent() {
    return this->eventsHeap.peekMin();
}

Event* Scheduler::removeNextEvent() {
    return this->eventsHeap.extractMin();
}
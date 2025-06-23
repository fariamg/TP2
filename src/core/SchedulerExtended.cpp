#include "../../include/core/SchedulerExtended.h"
#include "../../include/utils/Logger.h"
#include <algorithm>
#include <cmath>
#include <iostream>

SchedulerExtended::SchedulerExtended(const ConfigDataExtended& configData, int maxEvents)
    : eventsHeap(maxEvents), timer(), routeRecalculations(0), capacityOverflows(0), weightOverflows(0), alternativeRoutesUsed(0) {

    if (configData.packages == nullptr || configData.warehouses == nullptr || configData.graph == nullptr) {
        throw std::invalid_argument("Dados de configuração inválidos foram passados para o SchedulerExtended.");
    }

    // Copia parâmetros base
    this->transportCapacity = configData.transportCapacity;
    this->transportLatency = configData.transportLatency;
    this->transportInterval = configData.transportInterval;
    this->removalCost = configData.removalCost;
    this->activePackages = configData.numPackages;

    // Copia parâmetros estendidos
    this->variableTransportTime = configData.variableTransportTime;
    this->variablePackageWeight = configData.variablePackageWeight;
    this->variableTransportCapacity = configData.variableTransportCapacity;
    this->limitedStorageCapacity = configData.limitedStorageCapacity;
    this->multipleRoutes = configData.multipleRoutes;
    this->dynamicRouting = configData.dynamicRouting;

    // Insere eventos de chegada de pacotes
    for (int i = 0; i < configData.numPackages; ++i) {
        PackageExtended* package = configData.packages[i];
        if (package != nullptr) {
            this->eventsHeap.insert(new Event(package->getPostTime(), package));
        }
    }
}

SchedulerExtended::~SchedulerExtended() {
    // Limpa eventos restantes
    while (!eventsHeap.isEmpty()) {
        Event* event = eventsHeap.extractMin();
        delete event;
    }
}

void SchedulerExtended::runSimulation(WarehouseExtended** warehouses, GraphExtended* graph, int numWarehouses) {
    if (this->activePackages > 0) {
        initializeTransportEvents(graph, numWarehouses);
    }

    while (!this->eventsHeap.isEmpty()) {
        Event* event = removeNextEvent();
        if (event == nullptr)
            continue;

        if (event->time < this->timer.getTime())
            continue;

        this->timer.setTime(event->time);

        // Atualiza tempo do grafo para cálculos dinâmicos
        if (variableTransportTime) {
            graph->updateCurrentTime(event->time);
        }

        switch (event->type) {
        case EventType::PACKAGE_ARRIVAL:
            handlePackageArrival(event, warehouses, graph);
            break;
        case EventType::LEAVE_TRANSPORT:
            handleTransportDeparture(event, warehouses, graph);
            break;
        default:
            break;
        }

        delete event;
    }
}

void SchedulerExtended::initializeTransportEvents(GraphExtended* graph, int numWarehouses) {
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

void SchedulerExtended::handlePackageArrival(Event* event, WarehouseExtended** warehouses, GraphExtended* graph) {
    PackageExtended* package = static_cast<PackageExtended*>(event->package);
    if (!package)
        return;

    int currentWarehouseId = package->getRoute().peekFront();

    if (package->getRoute().getCurrentSize() == 1) {
        // Pacote chegou ao destino final
        package->setState(PackageState::DELIVERED);
        logPackageDelivered(timer.getTime(), package->getId(), currentWarehouseId);
        this->activePackages--;
    } else {
        // Roteamento dinâmico
        if (dynamicRouting && shouldRecalculateRoute(package, graph, timer.getTime())) {
            calculateDynamicRoute(package, graph, timer.getTime());
            routeRecalculations++;
        }

        package->getRoute().removeFront();
        int nextWarehouseId = package->getRoute().peekFront();

        // Tenta armazenar considerando capacidade limitada
        bool stored = false;
        if (limitedStorageCapacity) {
            stored = warehouses[currentWarehouseId]->storePackage(package, nextWarehouseId);
            if (!stored) {
                handleCapacityOverflow(package, warehouses, graph->getNumWarehouses());
                capacityOverflows++;
            }
        } else {
            // Armazenamento ilimitado (versão original)
            stored = warehouses[currentWarehouseId]->storePackage(package, nextWarehouseId);
        }

        if (stored) {
            package->setCurrentLocation(currentWarehouseId);
            package->setState(PackageState::STORED);
            logPackageStored(timer.getTime(), package->getId(), currentWarehouseId, nextWarehouseId);
        }
    }
}

void SchedulerExtended::handleTransportDeparture(Event* event, WarehouseExtended** warehouses, GraphExtended* graph) {
    int originId = event->originWarehouseId;
    int sectionId = event->destinationSectionId;
    WarehouseExtended* originWarehouse = warehouses[originId];

    // Agenda próximo evento de transporte se ainda há pacotes ativos
    if (this->activePackages > 0) {
        int nextTransportTime = event->time + this->transportInterval;
        this->eventsHeap.insert(new Event(nextTransportTime, originId, sectionId));
    }

    if (originWarehouse->isSectionEmpty(sectionId)) {
        return;
    }

    // Calcula capacidade dinâmica de transporte
    int dynamicCapacity = transportCapacity;
    if (variableTransportCapacity) {
        dynamicCapacity = calculateDynamicTransportCapacity(originId, sectionId, graph);
    }

    // Remove pacotes da seção
    int numPackagesInSection = originWarehouse->getSectionSize(sectionId);
    PackageExtended** lifoBuffer = new PackageExtended*[numPackagesInSection];

    for (int i = 0; i < numPackagesInSection; ++i) {
        lifoBuffer[i] = originWarehouse->retrievePackage(sectionId);
    }

    // Calcula tempo de remoção considerando peso dos pacotes
    double lastRemovalTime = event->time;
    for (int i = 0; i < numPackagesInSection; i++) {
        int removalCostAdjusted = removalCost;
        if (variablePackageWeight && lifoBuffer[i]) {
            // Tempo de remoção aumenta com o peso
            removalCostAdjusted = static_cast<int>(removalCost * lifoBuffer[i]->getWeightImpactFactor());
        }
        lastRemovalTime += removalCostAdjusted;
        if (lifoBuffer[i]) {
            logPackageRemoved(lastRemovalTime, lifoBuffer[i]->getId(), originId, sectionId);
        }
    }

    // Determina quantos pacotes transportar baseado na capacidade dinâmica
    int numToTransport = std::min(numPackagesInSection, dynamicCapacity);
    int numToRestore = numPackagesInSection - numToTransport;

    // Transporta pacotes
    for (int i = numPackagesInSection - 1; i >= numToRestore; --i) {
        PackageExtended* package = lifoBuffer[i];
        if (package) {
            logPackageInTransit(lastRemovalTime, package->getId(), originId, sectionId);

            // Calcula latência dinâmica baseada no peso e tempo
            int latency = transportLatency;
            if (variableTransportTime) {
                latency = graph->getDynamicLatency(originId, sectionId);
            }
            if (variablePackageWeight) {
                latency = static_cast<int>(latency * package->getWeightImpactFactor());
            }

            int arrivalTime = lastRemovalTime + latency;
            this->eventsHeap.insert(new Event(arrivalTime, package));
        }
    }

    // Restaura pacotes não transportados
    for (int i = numToRestore - 1; i >= 0; --i) {
        PackageExtended* package = lifoBuffer[i];
        if (package) {
            originWarehouse->storePackage(package, sectionId);
            logPackageRestored(lastRemovalTime, package->getId(), originId, sectionId);
        }
    }

    delete[] lifoBuffer;
}

Event* SchedulerExtended::removeNextEvent() {
    return eventsHeap.extractMin();
}

void SchedulerExtended::calculateDynamicRoute(PackageExtended* package, GraphExtended* graph, int currentTime) {
    if (!multipleRoutes) {
        return; // Apenas uma rota disponível
    }

    int origin = package->getCurrentLocation();
    int destination = package->getFinalDestination();

    // Encontra melhor rota considerando peso e tempo atual
    Route bestRoute = graph->findBestRoute(origin, destination, package->getWeight(), currentTime);

    if (bestRoute.path.getCurrentSize() > 0) {
        // Adiciona rota atual ao histórico
        package->addRouteToHistory(package->getRoute());

        // Define nova rota
        package->setRoute(std::move(bestRoute.path));
        package->updateLastRouteCalculation(currentTime);
        alternativeRoutesUsed++;
    }
}

bool SchedulerExtended::shouldRecalculateRoute(PackageExtended* package, GraphExtended* graph, int currentTime) {
    // Suprime warning de parâmetro não usado
    (void)graph;

    // Recalcula rota se:
    // 1. Muito tempo passou desde a última calculação
    // 2. Pacote requer manuseio especial
    // 3. Há congestionamento na rota atual

    int timeSinceLastCalc = currentTime - package->getLastRouteCalculation();

    return (timeSinceLastCalc > 100) ||                                                                // Recalcula a cada 100 unidades de tempo
           package->getRequiresSpecialHandling() || (package->getRouteHistory().getCurrentSize() > 3); // Muitas mudanças de rota
}

void SchedulerExtended::handleCapacityOverflow(PackageExtended* package, WarehouseExtended** warehouses, int numWarehouses) {
    int currentLocation = package->getCurrentLocation();
    WarehouseExtended* currentWarehouse = warehouses[currentLocation];

    // Estratégia 1: Tentar armazenamento secundário
    if (currentWarehouse->transferToSecondaryStorage(package, package->getRoute().peekFront(), warehouses)) {
        return;
    }

    // Estratégia 2: Encontrar armazém alternativo com capacidade
    for (int i = 0; i < numWarehouses; ++i) {
        if (i != currentLocation && warehouses[i]->canStorePackage(package, package->getRoute().peekFront())) {
            // Modifica rota para incluir armazém intermediário
            LinkedList newRoute = package->getRoute();
            newRoute.addFront(i);
            package->setRoute(std::move(newRoute));
            warehouses[i]->storePackage(package, package->getRoute().peekFront());
            alternativeRoutesUsed++;
            return;
        }
    }

    // Estratégia 3: Força armazenamento na fila de espera
    currentWarehouse->storeInWaitingQueue(package, package->getRoute().peekFront());
}

int SchedulerExtended::calculateDynamicTransportCapacity(int origin, int destination, GraphExtended* graph) {
    EdgeInfo edge = graph->getEdgeInfo(origin, destination);

    // Capacidade varia baseada no tempo e na rota específica
    double timeFactor = 1.0 + 0.2 * sin(timer.getTime() * 0.05);
    double routeFactor = static_cast<double>(edge.capacity) / transportCapacity;

    return static_cast<int>(transportCapacity * timeFactor * routeFactor);
}

void SchedulerExtended::getSimulationMetrics(int& routeRecalc, int& capOverflow, int& weightOverflow, int& altRoutes) {
    routeRecalc = routeRecalculations;
    capOverflow = capacityOverflows;
    weightOverflow = weightOverflows;
    altRoutes = alternativeRoutesUsed;
}
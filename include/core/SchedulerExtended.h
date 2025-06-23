#pragma once

#include "../dataStructures/GraphExtended.h"
#include "../domains/PackageExtended.h"
#include "../domains/WarehouseExtended.h"
#include "ConfigDataExtended.h"
#include "Scheduler.h"

//@ Classe que implementa o scheduler estendido com pontos extras
class SchedulerExtended {
  private:
    //@ Heap de eventos
    MinHeap eventsHeap;

    //@ Timer da simulação
    Timer timer;

    //@ Parâmetros de configuração estendidos
    int transportCapacity;
    int transportLatency;
    int transportInterval;
    int removalCost;
    int activePackages;

    //@ Novos parâmetros para pontos extras
    bool variableTransportTime;
    bool variablePackageWeight;
    bool variableTransportCapacity;
    bool limitedStorageCapacity;
    bool multipleRoutes;
    bool dynamicRouting;

    //@ Contadores para métricas
    int routeRecalculations;
    int capacityOverflows;
    int weightOverflows;
    int alternativeRoutesUsed;

  public:
    //@ Construtor
    SchedulerExtended(const ConfigDataExtended& configData, int maxEvents = 10000);

    //@ Destrutor
    ~SchedulerExtended();

    //@ Função principal para executar a simulação
    void runSimulation(WarehouseExtended** warehouses, GraphExtended* graph, int numWarehouses);

    //@ Função para obter métricas da simulação
    void getSimulationMetrics(int& routeRecalc, int& capOverflow, int& weightOverflow, int& altRoutes);

  private:
    //@ Função para inicializar eventos de transporte
    void initializeTransportEvents(GraphExtended* graph, int numWarehouses);

    //@ Função para lidar com chegada de pacotes
    void handlePackageArrival(Event* event, WarehouseExtended** warehouses, GraphExtended* graph);

    //@ Função para lidar com partida de transporte
    void handleTransportDeparture(Event* event, WarehouseExtended** warehouses, GraphExtended* graph);

    //@ Remove o próximo evento do heap
    Event* removeNextEvent();

    //@ Calcula rota dinâmica para um pacote
    void calculateDynamicRoute(PackageExtended* package, GraphExtended* graph, int currentTime);

    //@ Verifica se é necessário recalcular rota
    bool shouldRecalculateRoute(PackageExtended* package, GraphExtended* graph, int currentTime);

    //@ Lida com overflow de capacidade
    void handleCapacityOverflow(PackageExtended* package, WarehouseExtended** warehouses, int numWarehouses);

    //@ Calcula capacidade de transporte dinâmica
    int calculateDynamicTransportCapacity(int origin, int destination, GraphExtended* graph);
};
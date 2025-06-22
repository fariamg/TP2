#pragma once

#include "../include/io/InputLoader.h"
#include "dataStructures/MinHeap.h"
#include "utils/Timer.h"

static constexpr size_t MAX_EVENTS = 1000;

//@ Classe que implementa o escalonador de eventos
class Scheduler {
  private:
    //@ MinHeap que armazena os eventos a serem processados
    MinHeap eventsHeap;

    //@ Timer que controla o tempo dos eventos
    Timer timer;

    //@ Atributos que guardam as configurações gerais do sistema
    int transportCapacity, transportLatency, transportInterval, removalCost;

    int activePackages;

    //@ Função auxiliar que inicializa os eventos de transporte no grafo
    //@ @param graph Grafo que representa os armazéns e suas conexões
    //@ @param numWarehouses Número total de armazéns no sistema
    void initializeTransportEvents(Graph* graph, int numWarehouses);

    //@ FUnção que processa um evento de chegada de pacote
    //@ @param event Evento a ser processado
    //@ @param warehouses Array de armazéns do sistema
    void handlePackageArrival(Event* event, Warehouse** warehouses);

    //@ Função que processa um evento de transporte
    //@ @param event Evento a ser processado
    //@ @param warehouses Array de armazéns do sistema
    void handleTransportDeparture(Event* event, Warehouse** warehouses);

  public:
    // TODO: ver se faz sentido essa limite de eventos
    //@ Construtor que inicializa o escalonador
    // @param maxEvents Capacidade máxima de eventos no escalonador
    //@ @param configData Dados de configuração do sistema
    Scheduler(const IO::ConfigData& configData, int maxEvents = MAX_EVENTS);

    //@ Destrutor que libera os recursos alocados
    ~Scheduler();

    void runSimulation(Warehouse** warehouses, Graph* graph, int numWarehouses);
    
    //@ Função que retorna o próximo evento a ser executado
    Event* getNextEvent();

    //@ Getter que retorna o número total de eventos
    int getTotalEvents() const noexcept;

    //@ Função que retorna o tempo atual do relogio da simulação
    int getCurrentTime() const noexcept;

    //@ Adiciona um novo evento á fila de prioridade
    //@ @event Evento a ser adicionado
    void addEvent(Event* event);

    //@ Função que remove o próximo evento da fila de prioridade
    Event* removeNextEvent();

    //@ Função que avança o relogio da simulação
    //@ @param timeToAdd Tempo a ser adicinado no timer da simulação
    void advanceTimer(int timeToAdd);

    //@ Função que verifica se ainda há eventos a serem processados
    bool isEmpty() const noexcept;

    void printEvents();

    // void updatePreviousStorageTime(Package** packages, int lastIndex, int maxTime);

    // void initialStoragesInWarehouse(Warehouse** warehouses) noexcept;

    // void initializeTransportEvents(Warehouse** warehouses, int numPackages) noexcept;
};
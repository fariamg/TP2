#pragma once

#include "../dataStructures/Graph.h"
#include "../dataStructures/MinHeap.h"

//@ Forward declaration das classes usadas
class Warehouse;
struct ConfigData;

//@ Classe que implementa um timer para controlar o tempo da simulação
class Timer {
  private:
    //@ Atributos que guardam inicio e fim
    int time;

  public:
    //@ Contrutor padrão
    Timer();

    //@ Função que retorna o tempo atual acumulado
    int getTime() const noexcept;

    //@ Função que define o tempo atual do timer
    void setTime(int newTime);

    //@ Função que adiciona tempo ao timer
    //@ @param time Tempo a ser adicionado
    void addTime(int time);
};

//* Constante que define a capacidade máxima de eventos no escalonador
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

    //@ Atributo que guarda o número de pacotes ativos no sistema
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
    //@ Construtor que inicializa o escalonador
    // @param maxEvents Capacidade máxima de eventos no escalonador
    //@ @param configData Dados de configuração do sistema
    Scheduler(const ConfigData& configData, int maxEvents = MAX_EVENTS);

    //@ Destrutor que libera os recursos alocados
    ~Scheduler();

    //@ Função que inicia a simulação
    //@ @param warehouses Array de armazéns do sistema
    //@ @param graph Grafo que representa os armazéns e suas conexões
    //@ @param numWarehouses Número total de armazéns no sistema
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
};


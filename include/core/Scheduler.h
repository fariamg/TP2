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

  public:
    // TODO: ver se faz sentido essa limite de eventos
    //@ Construtor que inicializa o escalonador
    // @param maxEvents Capacidade máxima de eventos no escalonador
    //@ @param configData Dados de configuração do sistema
    Scheduler(int maxEvents = MAX_EVENTS, const IO::ConfigData& configData);

    //@ Getter que retorna o número total de eventos
    int getTotalEvents() const noexcept;

    //@ Adiciona um novo evento á fila de prioridade
    //@ @event Evento a ser adicionado
    void addEvent(Event* event);

    //@ Função que retorna o próximo evento a ser executado
    Event* getNextEvent();

    //@ Função que remove o próximo evento da fila de prioridade
    void removeNextEvent();

    //@ Função que avança o relogio da simulação
    //@ @param timeToAdd Tempo a ser adicinado no timer da simulação
    void advanceTimer(int timeToAdd);

    //@ Função que retorna o tempo atual do relogio da simulação
    int getCurrentTime() const noexcept;

    //@ Função que verifica se ainda há eventos a serem processados
    bool isEmpty() const noexcept;
};
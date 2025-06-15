#pragma once

#include "utils/Timer.h"
#include "dataStructures/MinHeap.h"

static constexpr size_t MAX_EVENTS = 1000; 

//@ Classe que implementa o escalonador de eventos
class Scheduler {
private:
  //@ MinHeap que armazena os eventos a serem processados
  MinHeap eventsHeap;

  //@ Timer que controla o tempo dos eventos
  Timer timer;

  //@ Atributo que guarda o numero de eventos atuais
  int totalEvents;

public:
  // TODO: ver se faz sentido essa limite de eventos
  //@ Construtor que inicializa o escalonador
  Scheduler(int maxEvents);

  //@ Getter que retorna o número total de eventos
  int getTotalEvents() const noexcept;

  //@ Função que incrementa o número total de eventos
  void incrementTotalEvents() noexcept; 

  //@ Adiciona um novo evento á fila de prioridade
  //@ @event Evento a ser adicionado
  void addEvent(Event *event);

  //@ Função que retorna o próximo evento a ser executado
  Event *getNextEvent();

  //@ Função que remove o próximo evento da fila de prioridade
  void *removeNextEvent();

  //@ Função que avança o relogio da simulação
  //@ @param timeToAdd Tempo a ser adicinado no timer da simulação
  void advanceTimer(int timeToAdd);

  //@ Função que retorna o tempo atual do relogio da simulação
  int getCurrentTime() const noexcept;

  //@ Função que verifica se ainda há eventos a serem processados
  bool isEmpty() const noexcept;
};
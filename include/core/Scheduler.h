#pragma once

#include "MinHeap.h"
#include "Timer.h"

//@ Classe que implementa o escalonador de eventos
class Scheduler {
private:
  //@ MinHeap que armazena os eventos a serem processados
  MinHeap eventsHeap;

  //@ Timer que controla o tempo dos eventos
  Timer timer;

public:
  // TODO: ver se faz sentido essa limite de eventos
  //@ Construtor que inicializa o escalonador
  Scheduler(int maxEvents);

  //@ Adiciona um novo evento á fila de prioridade
  //@ @event Evento a ser adicionado
  void addEvent(Event *event);

  //@ Função qeu retorna o próximo evento a ser executado
  Event *getNextEvent();

  //@ Função que avança o relogio da simulação
  //@ @param timeToAdd Tempo a ser adicinado no timer da simulação
  void advanceTimer(int timeToAdd);

  //@ Função que retorna o tempo atual do relogio da simulação
  int getCurrentTime() const noexcept;

  //@ Função que verifica se ainda há eventos a serem processados
  bool isEmpty() const noexcept;
};
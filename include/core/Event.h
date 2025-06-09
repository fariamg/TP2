#pragma once

#include "EventType.h"
#include "Package.h"

class Event {
public:
  //@ Atributo que define o tipo de evento
  EventType type;

  //@ Atributo que define a duração do evento
  int time;

  //@ Atributo que define o pacote relacionado ao evento
  //@ Este atributo é usado para eventos de chegada de pacotes
  Package *package;

  //@ Atributos que definem os armazéns de origem e destino do evento
  //@ Esses atributos são usados para eventos de transporte
  int originWarehouseId, destinationWarehouseId;

  //@ Construtor para eventos de chegada de pacotes
  //@ @param time Duração do evento
  //@ @param package Pacote relacionado ao evento 
  Event(int time, Package *package);

  //@ Construtor para eventos de transporte
  //@ @param time Duração do evento
  //@ @param originWarehouseId ID do armazém de origem
  //@ @param destinationWarehouseId ID do armazém de destino
  Event(int time, int originWarehouseId, int destinationWarehouseId);

  //@ Sobrecarga de operador de comparação
  //@ Essencial para funcionamento do MinHeap
  //@ @param other Evento a ser comparado
  bool operator<(const Event &other) const noexcept;
};
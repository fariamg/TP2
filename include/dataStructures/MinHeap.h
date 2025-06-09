#pragma once

#include "Event.h"

//@ Classe de implementação de um MinHeap para o escalonador de eventos de maior prioridade
class MinHeap {
private:
  //@ Array de Event ponteiros que representa o heap
  Event **heapArray;

  //@ Capacidade máxima do heap e tamanho atual
  int capacity;
  int currentSize;

  //@ Funções auxiliares para manter a propriedade do heap
  //@ @param index Índice do nó a ser ajustado
  void heapifyUp(int index);
  void heapifyDown(int index);

  //@ Funções auxiliares para acessar os índices dos nós pai e filhos
  //@ @param index Índice do nó
  int parent(int index) const noexcept;
  int leftChild(int index) const noexcept;
  int rightChild(int index) const noexcept;

  //@ Função auxiliar para trocar dois eventos no heap
  //@ @param a Primeiro evento a ser trocado
  //@ @param b Segundo evento a ser trocado
  void swap(Event *&a, Event *&b) {};

public:
  //@ Construtor e destrutor
  //@ @param capacity Capacidade máxima do heap
  MinHeap(int capacity);
  ~MinHeap();

  //@ Função de inserção de eventos no heap
  //@ @param event Ponteiro para o evento a ser inserido
  void insert(Event *event);

  //@Função de extração do menor evento do heap
  Event *extractMin();

  //@ Função para espiar o menor evento do heap sem removê-lo
  Event *peekMin() const;

  //@ Função que retorna um booleno indicando se o heap está cheio
  bool isEmpty() const noexcept;
  // void clear();
  // void printHeap() const;
};
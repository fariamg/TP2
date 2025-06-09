#pragma once

#include "Package.h"
#include "Node.h"


// @ Classe que representa uma pilha de pacotes
class Stack {
private:
  //@ Armazena o tamanho atual da pilha
  int currentSize;

  //@ Ponteiro para o topo da pilha
  Node *top;

public:
  //@ Construtor e destrutor
  Stack();
  ~Stack();

  //@ Função que retorna a capacidade da pilha
  int getCapacity() const noexcept;

  //@ Função que retorna o tamanho atual da pilha
  int getCurrentSize() const noexcept;

  //@ Função que verifica se a pilha está vazia
  bool isEmpty() const noexcept;

  //@ Função que verifica se a pilha está cheia
  // bool isFull() const noexcept;

  //@ Função que adiciona um pacote à pilha
  void push(const Package &package);

  //@ Função que remove e retorna o pacote do topo da pilha
  Package pop();

  //@ Função que retorna o pacote do topo da pilha sem removê-lo
  Package peek() const;
};
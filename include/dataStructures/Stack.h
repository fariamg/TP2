#pragma once

#include "Package.h"

// @ Classe que representa uma pilha de pacotes armazenada no Warehouse
class Stack {
private:
  //@ Struct de implemtação de um nó
  struct Node {
    //@ Ponteiro para o pacote armazenado no nó
    const Package *package;

    //@ Ponteiro para o próximo nó na pilha
    Node *next;
  };
  
  //@ Armazena o tamanho atual da pilha
  int currentSize;

  //@ Ponteiro para o topo da pilha
  Node *top;

public:
//TODO: ADICIONAR CAPACIDADE MAXIMA DE ARMAZENAMENTO NO CONSTRUTOR DA PILHA (PONTO EXTRA)
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
  //@ @param package Ponteiro para o pacote a ser adicionado
  void push(const Package *package);

  //@ Função que remove e retorna o pacote do topo da pilha
  const Package* pop();

  //@ Função que retorna o pacote do topo da pilha sem removê-lo
  const Package* peek() const;
};
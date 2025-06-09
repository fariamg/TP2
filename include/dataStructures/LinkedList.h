#pragma once

#include "Warehouse.h"

//@ Classe que implementa uma Linked List para armazenar a rota ótima entre
//@ armazéns para certo pacote
class LinkedList {
private:
  //@ Struct que define um nó da lista ligada, com seu respectivo
  //@ armazém e ponteiro para o próximo nó
  struct Node {
    //@ Indice do armazém armazenado no nó
    int warehouseIndex;

    //@ Ponteiro para o próximo nó na lista
    Node *next;
  };

  //@ Ponteiro para o início e fim da lista
  Node *head;
  Node *tail;

  //@ Atributo que guarda o tamanho atual da lista
  int currentSize;

public:
  //@ Construtor e destrutor
  LinkedList();
  ~LinkedList();

  //@ Função que retorna o tamanho atual da lista
  int getCurrentSize() const noexcept;

  //@ Função que remove o primeiro armazém da lista
  void removeFront();

  //@ Função que retorna o ID do primeiro armazem sem removê-lo
  int peekFront() const;

  //@ Função que adiciona um nó com o índice do armazém ao final da lista
  //@ @param warehouseIndex Índice do armazém a ser adicionado
  void addWarehouse(int& warehouseIndex);

  //@ Função que retorna um booleano indicando se a lista está vazia
  bool isEmpty() const noexcept;

  //@ Função que limpa a lista, removendo todos os nós
  // void clear() noexcept;
};
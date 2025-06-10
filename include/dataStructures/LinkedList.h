#pragma once

#include <stdexcept> 

//@ Classe que implementa uma lista ligada genérica, para rota ótima e lista de
//@ pilhas
template <typename T> class LinkedList {
private:
  //@ Struct que define um nó da lista ligada
  struct Node {
    //@ Data armazenado no nó
    T data;

    //@ Ponteiro para o próximo nó na lista
    Node *next;

    //@ Construtor do nó que inicializa o dado e o ponteiro para o próximo nó
    Node(const T &item) : data(item), next(nullptr) {}
  };

  //@ Ponteiro para o início e fim da lista
  Node *head;
  Node *tail;

  //@ Atributo que guarda o tamanho atual da lista
  int currentSize;

public:
  //@ Construtor
  LinkedList() {
    this->head = nullptr;
    this->tail = nullptr;
    this->currentSize = 0;
  }

  //@ Destrutor
  ~LinkedList() {
    while (!isEmpty()) {
      removeFront();
    }
  }

  //@ Função que retorna o tamanho atual da lista
  int getCurrentSize() const noexcept { return this->currentSize; }

  //@ Função que retorna um booleano indicando se a lista está vazia
  bool isEmpty() const noexcept { return this->head == nullptr; }

  //@ Função que adiciona um novo nó ao final da lista
  void addBack(const T &data) {
    Node *newNode = new Node(data);

    if (isEmpty()) {
      this->head = newNode;
      this->tail = newNode;
    } else {
      this->tail->next = newNode;
      this->tail = newNode;
    }
    this->currentSize++;
  }

  //@ Função que adiciona um novo nó ao início da lista
  void addFront(const T &data) {
    Node *newNode = new Node(data);

    if (isEmpty()) {
      this->head = newNode;
      this->tail = newNode;
    } else {
      newNode->next = this->head;
      this->head = newNode;
    }
    this->currentSize++;
  }

  //@ Função que remove o primeiro nó da lista
  void removeFront() {
    if (isEmpty()) {
      return;
    }

    Node *nodeToDelete = this->head;
    this->head = this->head->next;
    delete nodeToDelete;
    this->currentSize--;

    if (isEmpty()) {
      this->tail = nullptr;
    }
  }

  //@ Função que retorna o dado do primeiro nó sem removê-lo
  T peekFront() const {
    if (isEmpty()) {
      throw std::out_of_range("Nao e possivel espiar uma lista vazia.");
    }
    return this->head->data;
  }
};
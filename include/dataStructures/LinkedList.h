#pragma once

#include <stdexcept>

//@ Classe que implementa uma lista ligada genérica, para rota ótima e lista de
//@ pilhas
class LinkedList {
  private:
    //@ Struct que define um nó da lista ligada
    struct Node {
        //@ Data armazenado no nó
        int data;

        //@ Ponteiro para o próximo nó na lista
        Node* next;

        //@ Construtor do nó que inicializa o dado e o ponteiro para o próximo nó
        Node(int data) : data(data), next(nullptr) {}
    };

    //@ Ponteiro para o início e fim da lista
    Node* head;
    Node* tail;

    //@ Atributo que guarda o tamanho atual da lista
    int currentSize;

  public:
    //@ Construtor
    LinkedList();

    //@ Construtor de cópia
    LinkedList(const LinkedList& other);

    //@ Destrutor
    ~LinkedList();

    //@ Sobrecarga do operador de atribuição
    LinkedList& operator=(const LinkedList& other);

    //@ Função que retorna o tamanho atual da lista
    int getCurrentSize() const noexcept;

    //@ Função que retorna um booleano indicando se a lista está vazia
    bool isEmpty() const noexcept;

    //@ Função que adiciona um novo nó ao final da lista
    void addBack(int data);

    //@ Função que adiciona um novo nó ao início da lista
    void addFront(int data);

    //@ Função que remove o primeiro nó da lista
    void removeFront();

    //@ Função que retorna o dado do primeiro nó sem removê-lo
    int peekFront() const;

    //@ Função que retorna um dado em uma posição específica da lista
    int getDataAt(int position) const;

    //@ Função que copia os dados de outra lista ligada
    void copy(const LinkedList& other);
};
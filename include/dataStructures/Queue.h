#pragma once

#include "../domains/Package.h"

//@ Classe que implementa uma fila de pacotes usando uma lista encadeada
class Queue {
private:
    //@ Estrutura interna que representa um nó na fila
    struct Node {
        Package* data; 
        Node* next;    

        Node(Package* dataValue);
    };

    //@ Ponteiros para o início e fim da fila, e o tamanho atual
    Node* front; 
    Node* rear; 
    int currentSize; 

public:
    
    //@ Construtor e destrutor
    Queue();
    ~Queue();

    //@ Função de inserção de pacotes na fila
    //@ @param data Ponteiro para o pacote a ser inserido
    void enqueue(Package* data);

    //@ Função de remoção de pacotes da fila
    //@ @return Ponteiro para o pacote removido
    Package* dequeue();

    //@ Função que retorna o pacote do início da fila sem removê-lo
    //@ @return Ponteiro para o pacote no início da fila
    Package* peek() const;

    //@ Função que verifica se a fila está vazia
    //@ @return true se a fila estiver vazia, false caso contrário
    bool isEmpty() const;

    //@ Função que retorna o tamanho atual da fila
    //@ @return Tamanho da fila
    int getSize() const;
};
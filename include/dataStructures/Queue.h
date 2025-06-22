#pragma once

// Incluímos Package.h porque a fila agora depende diretamente deste tipo.
#include "../domains/Package.h"

/**
 * @class Queue
 * @brief Implementação de um TAD de Fila (Queue) para ponteiros de Pacote (Package*).
 *
 * Esta fila segue a política FIFO (First-In, First-Out) e é implementada
 * com uma lista encadeada para ter tamanho dinâmico.
 */
class Queue {
private:
    /**
     * @struct Node
     * @brief Um nó interno da lista encadeada que compõe a fila.
     */
    struct Node {
        Package* data; // O dado armazenado é um ponteiro para um Pacote.
        Node* next;    // Ponteiro para o próximo nó na fila.

        // Construtor do nó para facilitar a criação.
        Node(Package* dataValue);
    };

    Node* front; // Ponteiro para o início da fila (de onde se remove).
    Node* rear;  // Ponteiro para o fim da fila (onde se insere).
    int currentSize; // Tamanho atual da fila.

public:
    // --- Construtor e Destrutor ---
    
    /**
     * @brief Construtor padrão. Cria uma fila vazia.
     */
    Queue();

    /**
     * @brief Destrutor. Libera toda a memória alocada pelos nós da fila.
     */
    ~Queue();

    // --- Operações Principais ---

    /**
     * @brief Insere um ponteiro de Pacote no final da fila (enqueue).
     * @param data O ponteiro para o Pacote a ser inserido.
     */
    void enqueue(Package* data);

    /**
     * @brief Remove e retorna o ponteiro de Pacote do início da fila (dequeue).
     * @return O ponteiro para o Pacote que estava no início da fila.
     */
    Package* dequeue();

    // --- Métodos Auxiliares ---

    /**
     * @brief Retorna o ponteiro de Pacote no início da fila sem removê-lo (peek).
     * @return O ponteiro para o Pacote que está no início da fila.
     */
    Package* peek() const;

    /**
     * @brief Verifica se a fila está vazia.
     * @return true se a fila não contiver elementos, false caso contrário.
     */
    bool isEmpty() const;

    /**
     * @brief Retorna o número de elementos na fila.
     * @return O tamanho atual da fila.
     */
    int getSize() const;
};
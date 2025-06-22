#include "../../include/dataStructures/Queue.h"
#include <stdexcept> // Para std::out_of_range

// --- Implementação do Nó ---

Queue::Node::Node(Package* dataValue) : data(dataValue), next(nullptr) {}

// --- Construtor e Destrutor ---

Queue::Queue() : front(nullptr), rear(nullptr), currentSize(0) {}

Queue::~Queue() {
    // Percorre a lista, deletando cada nó um por um para evitar vazamento de memória.
    Node* current = front;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

// --- Operações Principais ---

void Queue::enqueue(Package* data) {
    Node* newNode = new Node(data);
    if (isEmpty()) {
        // Se a fila está vazia, o novo nó é tanto o início quanto o fim.
        front = newNode;
        rear = newNode;
    } else {
        // Se não, o novo nó é inserido após o último elemento atual.
        rear->next = newNode;
        rear = newNode; // O novo nó se torna o novo fim da fila.
    }
    currentSize++;
}

Package* Queue::dequeue() {
    if (isEmpty()) {
        throw std::out_of_range("A Fila está vazia. Não é possível remover.");
    }

    Node* temp = front;         // Guarda o nó da frente para deletá-lo.
    Package* data = front->data;   // Guarda o ponteiro para o Pacote para retorná-lo.
    
    front = front->next; // O novo início da fila é o segundo elemento.
    delete temp;                // Libera a memória do antigo nó da frente.
    
    currentSize--;

    // Caso especial: se a fila ficou vazia, o ponteiro 'rear' também deve ser nulo.
    if (front == nullptr) {
        rear = nullptr;
    }

    return data;
}

// --- Métodos Auxiliares ---

Package* Queue::peek() const {
    if (isEmpty()) {
        throw std::out_of_range("A Fila está vazia.");
    }
    return front->data;
}

bool Queue::isEmpty() const {
    return currentSize == 0;
}

int Queue::getSize() const {
    return currentSize;
}
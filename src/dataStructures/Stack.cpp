#include "../include/dataStructures/Stack.h"
#include <stdexcept>

Stack::Stack() : currentSize(0), top(nullptr) {}

Stack::~Stack() {
    while (top != nullptr) {
        Node* nodeToDelete = top;
        top = top->next;

        delete nodeToDelete;
    }
}

Stack::Stack(const Stack& other) : top(nullptr), currentSize(0) {
    // Se a outra pilha (a original) estiver vazia, não há nada a fazer.
    // A nova pilha já foi inicializada como vazia.
    if (other.isEmpty()) {
        return;
    }

    // 1. Copia o primeiro nó (o topo da pilha original).
    // Alocamos um nó completamente novo para a nossa pilha.
    this->top = new Node();
    this->top->package = other.top->package; // Copiamos o ponteiro do pacote.
    this->currentSize = 1;

    // 2. Prepara os ponteiros para percorrer as duas listas em paralelo.
    Node* otherCurrent = other.top->next; // Ponteiro para percorrer a pilha original.
    Node* thisCurrent = this->top;        // Ponteiro para construir a nova pilha.

    // 3. Percorre o resto da pilha original, criando novos nós e copiando os dados.
    while (otherCurrent != nullptr) {
        // Cria um novo nó e o anexa ao final da nossa nova lista.
        thisCurrent->next = new Node();
        thisCurrent = thisCurrent->next; // Avança para o novo nó recém-criado.

        // Copia o ponteiro do pacote do nó original para o novo nó.
        thisCurrent->package = otherCurrent->package;

        // Avança para o próximo nó na pilha original.
        otherCurrent = otherCurrent->next;
        this->currentSize++;
    }

    // Garante que o final da nossa nova lista aponte para nullptr.
    thisCurrent->next = nullptr;
}

int Stack::getCurrentSize() const noexcept {
    return this->currentSize;
}

bool Stack::isEmpty() const noexcept {
    return this->top == nullptr;
}

void Stack::push(Package* package) {
    Node* newNode = new Node();
    newNode->package = package;

    newNode->next = top;
    top = newNode;

    currentSize++;
}

Package* Stack::pop() {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }

    Node* temp = top;
    Package* poppedPackage = top->package;
    top = top->next;

    delete temp;

    currentSize--;

    return poppedPackage;
}

const Package* Stack::peek() const {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }

    return top->package;
}

Package* Stack::peek(int index) const {
    if (index < 0 || index >= currentSize) {
        throw std::out_of_range("Index out of range");
    }

    Node* currentNode = top;
    for (int i = 0; i < index; ++i) {
        currentNode = currentNode->next;
    }

    return currentNode->package;
}

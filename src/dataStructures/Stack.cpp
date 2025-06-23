#include "../../include/dataStructures/Stack.h"
#include <stdexcept>


Stack::Stack() : currentSize(0), top(nullptr) {}

Stack::~Stack() {
    while (!isEmpty()) {
        Node* nodeToDelete = top;
        top = top->next;
        delete nodeToDelete;
    }
}

Stack::Stack(const Stack& other) : currentSize(0), top(nullptr) {
    // Se a outra pilha (a original) estiver vazia, não há nada a fazer.
    // A nova pilha já foi inicializada como vazia.
    if (other.isEmpty()) {
        return;
    }

    // Constrói uma pilha temporária para manter a ordem dos elementos.
    Stack tempStack;

    // Transfere todos os elementos da pilha original para a pilha temporária.
    Node* current = other.top;
    while (current != nullptr) {
        tempStack.push(current->package);
        current = current->next;
    }

    // Transfere os elementos da pilha temporária para esta pilha.
    // Como as pilhas são LIFO, fazer isso duas vezes mantém a ordem original.
    while (!tempStack.isEmpty()) {
        this->push(tempStack.pop());
    }
}

int Stack::getCapacity() const noexcept {
    // For now, return unlimited capacity
    // This could be modified to implement limited capacity (extra points)
    return -1; // -1 indicates unlimited capacity
}

int Stack::getCurrentSize() const noexcept {
    return currentSize;
}

bool Stack::isEmpty() const noexcept {
    return top == nullptr;
}

void Stack::push(Package* package) {
    Node* newNode = new Node;
    newNode->package = package;
    newNode->next = top;
    top = newNode;
    currentSize++;
}

Package* Stack::pop() {
    if (isEmpty()) {
        throw std::out_of_range("Pilha vazia: Não é possível executar pop.");
    }

    Node* nodeToDelete = top;
    Package* package = top->package;
    top = top->next;
    delete nodeToDelete;
    currentSize--;

    return package;
}

const Package* Stack::peek() const {
    if (isEmpty()) {
        throw std::out_of_range("Pilha vazia: Não é possível executar peek.");
    }
    return top->package;
}

Package* Stack::peek(int index) const {
    if (isEmpty() || index < 0 || index >= currentSize) {
        return nullptr;
    }

    Node* current = top;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current->next;
    }

    return (current != nullptr) ? current->package : nullptr;
}

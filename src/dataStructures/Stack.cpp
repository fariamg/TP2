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

int Stack::getCurrentSize() const noexcept {
    return this->currentSize;
}

bool Stack::isEmpty() const noexcept {
    return this->top == nullptr;
}

void Stack::push(const Package* package) {
    Node* newNode = new Node();
    newNode->package = package;

    newNode->next = top;
    top = newNode;

    currentSize++;
}

const Package* Stack::pop() {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }

    Node* temp = top;
    const Package* poppedPackage = top->package;
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

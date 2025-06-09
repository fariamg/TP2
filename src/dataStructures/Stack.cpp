#include "../dataStructures/Stack.h"
#include <stdexcept>

Stack::Stack() : currentSize(0), top(nullptr) {}

Stack::~Stack() {
  while (top != nullptr) {
    Node* nodeToDelete = top;
    top = top->next;
    
    delete nodeToDelete; 
  }
}

int Stack::getCurrentSize() const noexcept { return this->currentSize; }

bool Stack::isEmpty() const noexcept { return this->top == nullptr; }

void Stack::push(const Package &package) {
  Node *newNode = new Node;
  newNode->package = new Package(package);
  top->next = newNode;
  top = newNode;

  currentSize++;
}

Package Stack::pop() {
  if (isEmpty()) {
    throw std::underflow_error("Stack is empty");
  }

  Node *temp = top;
  Package* poppedPackage = top->package;
  top = top->next;

  delete temp;

  currentSize--;

  return *poppedPackage;
}

Package Stack::peek() const {
  if (isEmpty()) {
    throw std::underflow_error("Stack is empty");
  }

  return *top->package;
}

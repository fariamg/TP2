#include "../dataStructures/Stack.h"
#include <stdexcept>

Stack::Stack(int capacity) : capacity(capacity), top(nullptr) {
  packages = new Package[capacity];
}

Stack::~Stack() { delete[] packages; }

int Stack::getCapacity() const noexcept { return this->capacity; }

int Stack::getCurrentSize() const noexcept { return this->currentSize; }

bool Stack::isEmpty() const noexcept { return this->top == nullptr; }

bool Stack::isFull() const noexcept { return this->currentSize == capacity; }

void Stack::push(const Package &package) {
  if (isFull()) {
    throw std::overflow_error("Stack is full");
  }
  packages[currentSize++] = package;
  top = &packages[currentSize - 1];
}

Package Stack::pop() {
  if (isEmpty()) {
    throw std::underflow_error("Stack is empty");
  }
  Package poppedPackage = *top;
  currentSize--;
  top = (currentSize > 0) ? &packages[currentSize - 1] : nullptr;
  return poppedPackage;
}

Package Stack::peek() const {
  if (isEmpty()) {
    throw std::underflow_error("Stack is empty");
  }
  return *top;
}

void Stack::clear() {
  currentSize = 0;
  top = nullptr;
}

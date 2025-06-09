#include "../include/dataStructures/MinHeap.h"
#include <stdexcept>

MinHeap::MinHeap(int capacity) : capacity(capacity), currentSize(0) {
  heapArray = new Event *[capacity];
}

//! Deleta apenas a memoria do array de ponteiros, não os eventos
//! (responsabilidade unica)
MinHeap::~MinHeap() { delete[] heapArray; }

// TODO: SOBRECARREGAR OPERATOR NA CLASSE EVENT PARA COMPARAÇÃO
void MinHeap::heapifyUp(int index) {
  while (index > 0 && *heapArray[index] < *heapArray[parent(index)]) {
    swap(heapArray[index], heapArray[parent(index)]);
    index = parent(index);
  }
}

void MinHeap::heapifyDown(int index) {
  int smallest = index;
  int left = leftChild(index);
  int right = rightChild(index);

  if (left < currentSize && *heapArray[left] < *heapArray[smallest]) {
    smallest = left;
  }

  if (right < currentSize && *heapArray[right] < *heapArray[smallest]) {
    smallest = right;
  }

  if (smallest != index) {
    swap(heapArray[index], heapArray[smallest]);
    heapifyDown(smallest);
  }
}

int MinHeap::parent(int index) const noexcept { return (index - 1) / 2; }

int MinHeap::leftChild(int index) const noexcept { return 2 * index + 1; }

int MinHeap::rightChild(int index) const noexcept { return 2 * index + 2; }

void MinHeap::swap(Event *&a, Event *&b) {
  if (a == nullptr || b == nullptr) {
    throw std::invalid_argument("Cannot swap null pointers");
  }

  Event *temp = a;
  a = b;
  b = temp;
}

bool MinHeap::isEmpty() const noexcept { return currentSize == 0; }

void MinHeap::insert(Event *event) {
  if (currentSize >= capacity) {
    throw std::overflow_error("Heap is full");
  }

  int index = currentSize - 1;
  heapArray[index] = event;

  heapifyUp(index);

  currentSize++;
}

Event *MinHeap::extractMin() {
  if (isEmpty()) {
    throw std::out_of_range("Heap is empty");
  }

  if (currentSize == 1) {
    currentSize--;
    return heapArray[0];
  }

  Event *minEvent = heapArray[0];
  heapArray[0] = heapArray[currentSize - 1];
  currentSize--;

  heapifyDown(0);

  return minEvent;
}

Event *MinHeap::peekMin() const {
  if (isEmpty()) {
    throw std::out_of_range("Heap is empty");
  }

  return heapArray[0];
}

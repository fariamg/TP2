#include "LinkedList.h"
#include <stdexcept>

LinkedList::LinkedList() : head(nullptr), tail(nullptr), currentSize(0) {}

LinkedList::~LinkedList() {
  Node *current = head;
  while (current != nullptr) {
    Node *nextNode = current->next;
    delete current;
    current = nextNode;
  }
}

int LinkedList::getCurrentSize() const noexcept { return currentSize; }

void LinkedList::removeFront() {
  if (isEmpty()) {
    throw std::out_of_range("A lista está vazia");
  }

  Node *temp = head;
  head = head->next;
  delete temp;

  currentSize--;

  if (isEmpty()) {
    tail = nullptr;
  }
}

int LinkedList::peekFront() const {
  if (isEmpty()) {
    throw std::out_of_range("A lista está vazia");
  }

  return head->warehouseIndex;
}

void LinkedList::addWarehouse(int& warehouseIndex)  {
    Node *newNode = new Node;
    newNode->warehouseIndex = warehouseIndex;
    newNode->next = nullptr;
    
    if (isEmpty()) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    
    currentSize++;
}

bool LinkedList::isEmpty() const noexcept {
  return currentSize == 0;
}

#include "../include/dataStructures/LinkedList.h"

LinkedList::LinkedList() {
  this->head = nullptr;
  this->tail = nullptr;
  this->currentSize = 0;
}

LinkedList::~LinkedList() {
  while (!isEmpty()) {
    removeFront();
  }
}

int LinkedList::getCurrentSize() const noexcept {
  return this->currentSize;
}

bool LinkedList::isEmpty() const noexcept {
  return this->head == nullptr;
}

void LinkedList::addBack(int data) {
  Node *newNode = new Node(data);

  if (isEmpty()) {
    this->head = newNode;
    this->tail = newNode;
  } else {
    this->tail->next = newNode;
    this->tail = newNode;
  }
  this->currentSize++;
}

void LinkedList::addFront(int data) {
  Node *newNode = new Node(data);

  if (isEmpty()) {
    this->head = newNode;
    this->tail = newNode;
  } else {
    newNode->next = this->head;
    this->head = newNode;
  }
  this->currentSize++;
}

void LinkedList::removeFront() {
  if (isEmpty()) {
    return;
  }

  Node *nodeToDelete = this->head;
  this->head = this->head->next;
  delete nodeToDelete;
  this->currentSize--;

  if (isEmpty()) {
    this->tail = nullptr;
  }
}

int LinkedList::peekFront() const {
  if (isEmpty()) {
    throw std::out_of_range("Nao e possivel espiar uma lista vazia.");
  }
  return this->head->data;
}
#include "../include/dataStructures/LinkedList.h"

LinkedList::LinkedList() {
    this->head = nullptr;
    this->tail = nullptr;
    this->currentSize = 0;
}

LinkedList::LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), currentSize(0) {
    Node* temp = other.head;
    while (temp != nullptr) {
        addBack(temp->data);
        temp = temp->next;
    }
}

LinkedList::~LinkedList() {
    while (!isEmpty()) {
        removeFront();
    }
}

LinkedList& LinkedList::operator=(const LinkedList& other) {
    if (this == &other) {
        return *this;
    }

    while (!isEmpty()) {
        removeFront();
    }

    Node* temp = other.head;
    while (temp != nullptr) {
        addBack(temp->data);
        temp = temp->next;
    }

    return *this;
}

int LinkedList::getCurrentSize() const noexcept {
    return this->currentSize;
}

bool LinkedList::isEmpty() const noexcept {
    return this->head == nullptr;
}

void LinkedList::addBack(int data) {
    Node* newNode = new Node(data);

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
    Node* newNode = new Node(data);

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

    Node* nodeToDelete = this->head;
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

void LinkedList::copy(const LinkedList& other) {
    if (this != &other) {
        while (!isEmpty()) {
            removeFront();
        }

        Node* current = other.head;
        while (current != nullptr) {
            addBack(current->data);
            current = current->next;
        }
    }
}

void LinkedList::removeBack() {
    if (isEmpty()) {
        return;
    }

    if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
    } else {
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = nullptr;
    }
    currentSize--;
}
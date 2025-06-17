#include "../include/dataStructures/MinHeap.h"
#include <iostream>
#include <stdexcept>

MinHeap::MinHeap(int capacity) : capacity(capacity), currentSize(0) {
    heapArray = new Event*[capacity];
}

//! Deleta apenas a memoria do array de ponteiros, não os eventos
//! (responsabilidade unica)
MinHeap::~MinHeap() {
    delete[] heapArray;
}

int MinHeap::getCurrentSize() const noexcept {
    return currentSize;
}

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

int MinHeap::parent(int index) const noexcept {
    return (index - 1) / 2;
}

int MinHeap::leftChild(int index) const noexcept {
    return 2 * index + 1;
}

int MinHeap::rightChild(int index) const noexcept {
    return 2 * index + 2;
}

void MinHeap::swap(Event*& a, Event*& b) {
    if (a == nullptr || b == nullptr) {
        throw std::invalid_argument("Cannot swap null pointers");
    }

    Event* temp = a;
    a = b;
    b = temp;
}

bool MinHeap::isEmpty() const noexcept {
    return currentSize == 0;
}

void MinHeap::insert(Event* event) {
    if (currentSize >= capacity) {
        throw std::overflow_error("Heap is full");
    }

    heapArray[currentSize] = event;

    heapifyUp(currentSize);

    currentSize++;
}

Event* MinHeap::extractMin() {
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty");
    }

    Event* minEvent = heapArray[0];
    if (currentSize == 1) {
        currentSize--;
        return minEvent;
    } else {
        heapArray[0] = heapArray[currentSize - 1];
        currentSize--;
        heapifyDown(0);
        return minEvent;
    }
    return nullptr;
}

Event* MinHeap::peekMin() const {
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty");
    }

    return heapArray[0];
}

Event* MinHeap::peekMax() const {
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty");
    }

    Event* maxEvent = heapArray[0];
    for (int i = 1; i < currentSize; ++i) {
        if (*maxEvent < *heapArray[i]) {
            maxEvent = heapArray[i];
        }
    }
    return maxEvent;
}

void MinHeap::printHeap() noexcept {
    while (!isEmpty()) {
        Event* event = extractMin();
        std::cout << "Event Type: " << event->type << ", Time: " << event->time;
        if (event->type == STORAGE || event->type == REMOVAL) {
            std::cout << ", Package ID: " << event->package->getId() << ", Time stored: " << event->package->getTimeStored() << ", Current Location: " << event->package->getCurrentLocation();
        } else {
            std::cout << ", Origin Warehouse ID: " << event->originWarehouseId << ", Destination Warehouse ID: " << event->destinationWarehouseId;
        }
        std::cout << std::endl;
    }
}

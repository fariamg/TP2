#include "../../include/dataStructures/MinHeap.h"
#include <iostream>  // Permitido para std::cout
#include <stdexcept> // Permitido para tratamento de erros robusto

// --- Construtor e Destrutor ---

MinHeap::MinHeap(int capacity) : capacity(capacity), currentSize(0) {
    if (capacity <= 0) {
        throw std::invalid_argument("A capacidade do Heap deve ser um número positivo.");
    }
    heapArray = new Event*[capacity];
}

MinHeap::~MinHeap() {
    delete[] heapArray;
}

// --- Funções Públicas ---

void MinHeap::insert(Event* event) {
    if (currentSize >= capacity) {
        throw std::overflow_error("O Heap está cheio. Não é possível inserir.");
    }

    // Ordem correta das operações:
    // 1. Atribui o novo evento.
    heapArray[currentSize] = event;
    // 2. Incrementa o tamanho.
    currentSize++;
    // 3. Corrige a posição do novo elemento.
    heapifyUp(currentSize - 1);
}

Event* MinHeap::extractMin() {
    if (isEmpty()) {
        throw std::out_of_range("O Heap está vazio.");
    }

    // Lógica correta e simplificada:
    Event* minEvent = heapArray[0];
    heapArray[0] = heapArray[currentSize - 1];
    currentSize--;

    if (!isEmpty()) {
        heapifyDown(0);
    }
    
    return minEvent;
}

Event* MinHeap::peekMin() const {
    if (isEmpty()) {
        throw std::out_of_range("O Heap está vazio.");
    }
    return heapArray[0];
}

Event* MinHeap::peekMax() const {
    if (isEmpty()) {
        throw std::out_of_range("O Heap está vazio.");
    }
    
    Event* maxEvent = heapArray[0];
    for (int i = 1; i < currentSize; ++i) {
        if (*maxEvent < *heapArray[i]) {
            maxEvent = heapArray[i];
        }
    }
    return maxEvent;
}

int MinHeap::getCurrentSize() const noexcept {
    return currentSize;
}

bool MinHeap::isEmpty() const noexcept {
    return currentSize == 0;
}

// --- Funções Privadas Auxiliares ---

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
    // ALTERAÇÃO: Usando a implementação manual de swap conforme solicitado.
    Event* temp = a;
    a = b;
    b = temp;
}

// --- Função de Impressão para Debug ---

// Função de impressão não-destrutiva e segura.
// Não é noexcept porque a impressão pode (teoricamente) falhar e lançar exceções.
// É const porque não modifica o estado do heap.
void MinHeap::printHeap() const {
    std::cout << "--- Conteudo do Heap (Tamanho: " << currentSize << ") ---" << std::endl;
    for (int i = 0; i < currentSize; ++i) {
        Event* event = heapArray[i];
        if (event == nullptr) continue;

        std::cout << "Indice " << i << ": ";
        
        // Usa os tipos de evento corretos do Event.h corrigido
        if (event->type == EventType::PACKAGE_ARRIVAL) {
            // Checa se o pacote não é nulo antes de acessá-lo.
            if (event->package) {
                std::cout << "PACKAGE_ARRIVAL, Tempo: " << event->time 
                          << ", PkgID: " << event->package->getId();
            }
        } else if (event->type == EventType::LEAVE_TRANSPORT) {
            std::cout << "LEAVE_TRANSPORT, Tempo: " << event->time
                      << ", De: " << event->originWarehouseId 
                      << ", Para Secao: " << event->destinationSectionId;
        }
        std::cout << std::endl;
    }
    std::cout << "------------------------------------------------" << std::endl;
}
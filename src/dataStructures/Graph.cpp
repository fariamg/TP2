#include "../include/dataStructures/Graph.h"
#include <stdexcept>

Graph::Graph(int numWarehouses) : numWarehouses(numWarehouses) {
    this->adjacencyMatrix = new int*[numWarehouses];

    for (int i = 0; i < numWarehouses; ++i) {
        this->adjacencyMatrix[i] = new int[numWarehouses]{0}; //* Inicializa com 0
    }
}

Graph::~Graph() {
    for (int i = 0; i < numWarehouses; ++i) {
        delete[] this->adjacencyMatrix[i];
    }
    delete[] this->adjacencyMatrix;
}

int Graph::getNumWarehouses() const noexcept {
    return this->numWarehouses;
}

void Graph::addEdge(int i, int j, int capacity) {
    if (i < 0 || i >= numWarehouses || j < 0 || j >= numWarehouses) {
        throw std::out_of_range("Índices fora do intervalo do grafo");
    }

    this->adjacencyMatrix[i][j] = capacity;
    this->adjacencyMatrix[j][i] = capacity;
}

bool Graph::hasEdge(int i, int j) const {
    if (i < 0 || i >= numWarehouses || j < 0 || j >= numWarehouses) {
        throw std::out_of_range("Índices fora do intervalo do grafo");
    }

    return this->adjacencyMatrix[i][j] > 0;
}

LinkedList Graph::getNeighbors(int warehouseIndex) const {
    for (int i = 0; i < numWarehouses; ++i) {
        if (i < 0 || i >= numWarehouses) {
            throw std::out_of_range("Índice fora do intervalo do grafo");
        }
    }

    LinkedList neighbors;

    for (int j = 0; j < numWarehouses; ++j) {
        if (this->adjacencyMatrix[warehouseIndex][j] > 0) {
            Warehouse neighbor(j, numWarehouses);
            neighbors.addBack(neighbor.getId());
        }
    }
    return neighbors;
}
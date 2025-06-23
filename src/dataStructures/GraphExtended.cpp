#include "../../include/dataStructures/GraphExtended.h"
#include <climits>
#include <cmath>

GraphExtended::GraphExtended(int numWarehouses) : numWarehouses(numWarehouses), currentTime(0), cacheValid(false) {
    // Inicializa matriz de informações de arestas
    edgeMatrix = new EdgeInfo*[numWarehouses];
    for (int i = 0; i < numWarehouses; ++i) {
        edgeMatrix[i] = new EdgeInfo[numWarehouses];
    }

    // Inicializa cache de rotas
    routeCache = new Route**[numWarehouses];
    for (int i = 0; i < numWarehouses; ++i) {
        routeCache[i] = new Route*[numWarehouses];
        for (int j = 0; j < numWarehouses; ++j) {
            routeCache[i][j] = nullptr;
        }
    }
}

GraphExtended::~GraphExtended() {
    // Limpa matriz de arestas
    for (int i = 0; i < numWarehouses; ++i) {
        delete[] edgeMatrix[i];
    }
    delete[] edgeMatrix;

    // Limpa cache de rotas
    for (int i = 0; i < numWarehouses; ++i) {
        for (int j = 0; j < numWarehouses; ++j) {
            delete routeCache[i][j];
        }
        delete[] routeCache[i];
    }
    delete[] routeCache;
}

int GraphExtended::getNumWarehouses() const noexcept {
    return numWarehouses;
}

void GraphExtended::updateCurrentTime(int time) {
    if (time != currentTime) {
        currentTime = time;
        invalidateCache(); // Invalida cache quando tempo muda
    }
}

void GraphExtended::addEdge(int i, int j, int capacity, int baseLatency, double timeMultiplier) {
    if (i < 0 || i >= numWarehouses || j < 0 || j >= numWarehouses) {
        return;
    }

    edgeMatrix[i][j] = EdgeInfo(capacity, baseLatency, timeMultiplier);
    edgeMatrix[j][i] = EdgeInfo(capacity, baseLatency, timeMultiplier);
    invalidateCache();
}

EdgeInfo GraphExtended::getEdgeInfo(int i, int j) const {
    if (i < 0 || i >= numWarehouses || j < 0 || j >= numWarehouses) {
        return EdgeInfo();
    }
    return edgeMatrix[i][j];
}

int GraphExtended::getDynamicLatency(int i, int j) const {
    if (i < 0 || i >= numWarehouses || j < 0 || j >= numWarehouses) {
        return INT_MAX;
    }

    EdgeInfo edge = edgeMatrix[i][j];
    if (!edge.isActive) {
        return INT_MAX;
    }

    // Calcula latência dinâmica baseada no tempo
    double timeFactor = 1.0 + 0.1 * sin(currentTime * 0.01); // Variação senoidal
    return static_cast<int>(edge.baseLatency * edge.timeMultiplier * timeFactor);
}

bool GraphExtended::hasEdge(int i, int j) const {
    if (i < 0 || i >= numWarehouses || j < 0 || j >= numWarehouses) {
        return false;
    }
    return edgeMatrix[i][j].isActive;
}

LinkedList GraphExtended::getNeighbors(int warehouseIndex) const {
    LinkedList neighbors;

    if (warehouseIndex < 0 || warehouseIndex >= numWarehouses) {
        return neighbors;
    }

    for (int j = 0; j < numWarehouses; ++j) {
        if (edgeMatrix[warehouseIndex][j].isActive) {
            neighbors.addBack(j);
        }
    }

    return neighbors;
}

LinkedList GraphExtended::findAllRoutes(int origin, int destination, int maxDepth) {
    LinkedList allRoutes;
    LinkedList currentPath;
    bool* visited = new bool[numWarehouses];

    for (int i = 0; i < numWarehouses; ++i) {
        visited[i] = false;
    }

    currentPath.addBack(origin);
    visited[origin] = true;

    dfsRoutes(origin, destination, currentPath, allRoutes, visited, 0, maxDepth);

    delete[] visited;
    return allRoutes;
}

Route GraphExtended::findBestRoute(int origin, int destination, int packageWeight, int currentTime) {
    // Verifica cache primeiro
    if (cacheValid && routeCache[origin][destination] != nullptr) {
        return *routeCache[origin][destination];
    }

    Route bestRoute;

    // Implementação de Dijkstra modificado considerando peso e tempo
    int* dist = new int[numWarehouses];
    int* parent = new int[numWarehouses];
    bool* visited = new bool[numWarehouses];

    for (int i = 0; i < numWarehouses; ++i) {
        dist[i] = INT_MAX;
        parent[i] = -1;
        visited[i] = false;
    }

    dist[origin] = 0;

    for (int count = 0; count < numWarehouses - 1; ++count) {
        int u = -1;
        for (int v = 0; v < numWarehouses; ++v) {
            if (!visited[v] && (u == -1 || dist[v] < dist[u])) {
                u = v;
            }
        }

        if (u == -1 || dist[u] == INT_MAX)
            break;

        visited[u] = true;

        for (int v = 0; v < numWarehouses; ++v) {
            if (!visited[v] && hasEdge(u, v)) {
                // Calcula custo considerando peso do pacote
                int baseCost = getDynamicLatency(u, v);
                double weightFactor = 1.0 + (packageWeight - 1) * 0.1;
                int totalCost = static_cast<int>(baseCost * weightFactor);

                if (dist[u] + totalCost < dist[v]) {
                    dist[v] = dist[u] + totalCost;
                    parent[v] = u;
                }
            }
        }
    }

    // Reconstroi rota
    if (dist[destination] != INT_MAX) {
        LinkedList path;
        int current = destination;
        while (current != -1) {
            path.addFront(current);
            current = parent[current];
        }

        bestRoute.path = path;
        bestRoute.totalLatency = dist[destination];
        bestRoute.priority = calculateRoutePriority(path, packageWeight, currentTime);

        // Salva no cache
        if (routeCache[origin][destination] != nullptr) {
            delete routeCache[origin][destination];
        }
        routeCache[origin][destination] = new Route(bestRoute);
    }

    delete[] dist;
    delete[] parent;
    delete[] visited;

    return bestRoute;
}

void GraphExtended::invalidateCache() {
    for (int i = 0; i < numWarehouses; ++i) {
        for (int j = 0; j < numWarehouses; ++j) {
            if (routeCache[i][j] != nullptr) {
                delete routeCache[i][j];
                routeCache[i][j] = nullptr;
            }
        }
    }
    cacheValid = false;
}

void GraphExtended::dfsRoutes(int current, int destination, LinkedList& currentPath, LinkedList& allRoutes, bool* visited, int depth, int maxDepth) {
    if (depth > maxDepth)
        return;

    if (current == destination) {
        // Adiciona rota encontrada à lista
        // (Implementação simplificada - na prática, seria necessário copiar o caminho)
        return;
    }

    for (int neighbor = 0; neighbor < numWarehouses; ++neighbor) {
        if (!visited[neighbor] && hasEdge(current, neighbor)) {
            visited[neighbor] = true;
            currentPath.addBack(neighbor);

            dfsRoutes(neighbor, destination, currentPath, allRoutes, visited, depth + 1, maxDepth);

            currentPath.removeBack();
            visited[neighbor] = false;
        }
    }
}

double GraphExtended::calculateRoutePriority(const LinkedList& path, int packageWeight, int currentTime) {
    // Calcula prioridade baseada em múltiplos fatores
    double priority = 0.0;

    // Fator de distância (menor distância = maior prioridade)
    int pathLength = path.getCurrentSize();
    priority += 100.0 / (pathLength + 1);

    // Fator de peso (pacotes mais pesados têm prioridade em rotas mais curtas)
    priority += (10.0 - packageWeight) * 5.0;

    // Fator temporal (considera congestionamento)
    double timeFactor = 1.0 + 0.2 * sin(currentTime * 0.02);
    priority *= timeFactor;

    return priority;
}
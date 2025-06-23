#pragma once

#include "LinkedList.h"

//@ Estrutura para armazenar informações de uma aresta
struct EdgeInfo {
    int capacity;
    int baseLatency;
    double timeMultiplier; // Multiplicador para variação temporal
    bool isActive;

    EdgeInfo() : capacity(0), baseLatency(0), timeMultiplier(1.0), isActive(false) {}
    EdgeInfo(int cap, int latency, double multiplier = 1.0) : capacity(cap), baseLatency(latency), timeMultiplier(multiplier), isActive(true) {}
};

//@ Estrutura para representar uma rota completa
struct Route {
    LinkedList path;
    int totalLatency;
    int totalCapacity;
    double priority;

    Route() : totalLatency(0), totalCapacity(0), priority(0.0) {}
};

//@ Classe que implementa o grafo estendido com múltiplas rotas e tempos variáveis
class GraphExtended {
  private:
    //@ Matriz de informações de arestas
    EdgeInfo** edgeMatrix;

    //@ Número de armazéns no grafo
    int numWarehouses;

    //@ Tempo atual da simulação para cálculos dinâmicos
    int currentTime;

    //@ Cache de rotas para otimização
    Route*** routeCache;
    bool cacheValid;

  public:
    //@ Construtor e destrutor
    GraphExtended(int numWarehouses);
    ~GraphExtended();

    //@ Função que retorna o número de armazéns no grafo
    int getNumWarehouses() const noexcept;

    //@ Atualiza o tempo atual para cálculos dinâmicos
    void updateCurrentTime(int time);

    //@ Função para adicionar uma aresta com informações estendidas
    void addEdge(int i, int j, int capacity, int baseLatency, double timeMultiplier = 1.0);

    //@ Função para obter informações de uma aresta
    EdgeInfo getEdgeInfo(int i, int j) const;

    //@ Função para calcular latência dinâmica baseada no tempo
    int getDynamicLatency(int i, int j) const;

    //@ Função que verifica a existência de uma aresta entre dois armazéns
    bool hasEdge(int i, int j) const;

    //@ Função que retorna os vizinhos de um armazém específico
    LinkedList getNeighbors(int warehouseIndex) const;

    //@ Função para encontrar todas as rotas possíveis entre dois armazéns
    LinkedList findAllRoutes(int origin, int destination, int maxDepth = 10);

    //@ Função para encontrar a melhor rota considerando múltiplos fatores
    Route findBestRoute(int origin, int destination, int packageWeight = 1, int currentTime = 0);

    //@ Função para invalidar cache de rotas (para roteamento dinâmico)
    void invalidateCache();

  private:
    //@ Função auxiliar para busca em profundidade de rotas
    void dfsRoutes(int current, int destination, LinkedList& currentPath, LinkedList& allRoutes, bool* visited, int depth, int maxDepth);

    //@ Função para calcular prioridade de uma rota
    double calculateRoutePriority(const LinkedList& path, int packageWeight, int currentTime);
};
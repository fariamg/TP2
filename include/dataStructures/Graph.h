#pragma once

#include "domains/Warehouse.h"

//@ Classe que implementa o grafo para representar os armazéns e suas conexões
class Graph {
  private:
    //@ Matriz de adjacência que representa o grafo
    int** adjacencyMatrix;

    //@ Número de armazéns no grafo
    int numWarehouses;

  public:
    //@ Construtor e destrutor
    // @param numWarehouses Número de armazéns no grafo
    Graph(int numWarehouses);
    ~Graph();

    //@ Função que retorna o número de armazéns no grafo
    int getNumWarehouses() const noexcept;

    //@ Função para adicionar uma aresta entre dois armazéns e a capacidade de transporte entre eles
    //@ @param i Índice do primeiro armazém
    //@ @param j Índice do segundo armazém
    //@ @param capacity Capacidade de transporte entre os armazéns
    // TODO: ADICIONAR UM PAIR JUNTO COM CAPCIDADE PARA REPRESENTAR O TEMPO ENTRE DOIS ARMAZENS SENDO VARIVEL A CADA CONEXÃO (PONTO EXTRA)
    void addEdge(int i, int j, int capacity);

    //@ Função que verifica a existência de uma aresta entre dois armazéns
    //@ @param i Índice do primeiro armazém
    //@ @param j Índice do segundo armazém
    bool hasEdge(int i, int j) const;

    //@Função que retorna os vizinhos de um armazém específico
    //@ @param warehouseIndex Índice do armazém
    LinkedList getNeighbors(int warehouseIndex) const;
};
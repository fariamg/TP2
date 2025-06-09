#pragma once

//@ Classe que implementa o grafo para representar os armazéns e suas conexões
class Graph {
private:
  //@ Matriz de adjacência que representa o grafo
  int **adjacencyMatrix;

  //@ Número de armazéns no grafo
  int numWarehouses;

public:
  //@ Construtor e destrutor
  Graph(int numWarehouses);
  ~Graph();

  //@ Função que retorna o número de armazéns no grafo
  int getNumWarehouses() const noexcept;

  //@ Função para adicionar uma aresta entre dois armazéns
  void addEdge(int i, int j);

  //@Função que retorna os vizinhos de um armazém específico
  //TODO:LinkedList getNeighbors(int warehouseIndex) const;
};
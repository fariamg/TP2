#pragma once

#include "LinkedList.h"

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
  //TODO: ADICIONAR UM PAIR JUNTO COM CAPCIDADE PARA REPRESENTAR O TEMPO ENTRE DOIS ARMAZENS SENDO VARIVEL A CADA CONEXÃO (PONTO EXTRA)
  void addEdge(int i, int j, int capacity);

  //@Função que retorna os vizinhos de um armazém específico
  LinkedList getNeighbors(int warehouseIndex) const;
};
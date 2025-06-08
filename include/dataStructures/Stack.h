#include "Package.h"

class Stack {
private:
  //@ Artributos que guardam a capacidade, tamanho atual
  int capacity, currentSize;

  //@ Ponteiro para o array de pacotes
  Package *packages;

  //@ Ponteiro para o topo da pilha
  Package *top;

public:
  //@ Construtor e destrutor
  Stack(int capacity);
  ~Stack();

  //@ Função que retorna a capacidade da pilha
  int getCapacity() const noexcept;

  //@ Função que retorna o tamanho atual da pilha
  int getCurrentSize() const noexcept;

  //@ Função que verifica se a pilha está vazia
  bool isEmpty() const noexcept;

  //@ Função que verifica se a pilha está cheia
  bool isFull() const noexcept;

  //@ Função que adiciona um pacote à pilha
  void push(const Package &package);

  //@ Função que remove e retorna o pacote do topo da pilha
  Package pop();

  //@ Função que retorna o pacote do topo da pilha sem removê-lo
  Package peek() const;

  //@ Função que limpa a pilha, removendo todos os pacotes
  void clear();
};
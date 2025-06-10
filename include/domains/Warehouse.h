#pragma once

#include "Stack.h"

//@ Classe que implementa o domínio armazem
class Warehouse {
private:
  //@ Atributo identificador do armazém
  int ID;

  //@ Pilhas de pacotes do armazém
  Stack* sessions;

  //@ Total de armazens no sistema
  int totalWarehouses;
public:
  //@ Construtor
  //@ @param ID Identificador único do armazém
  // TODO: PASSAR AQUI A CAPACIDADE DO ARMAZEM QUE PASSA PARA O CONSTRUTOR DA
  // PILHA INTERNO ( PONTO EXTRA)
  Warehouse(int ID, int totalWarehouses);
  //@ Destrutor
  ~Warehouse();

  //@ Retorna o ID do armazém
  int getId() const noexcept;

  //@ Armazena um pacote na seção correta (operação principal)
  void storePackage(const Package* pkg, int destinationId);

  //@ Recupera (remove) um pacote da seção de um destino (operação principal)
  const Package* retrievePackage(int destinationId);

  //@ Espia o pacote no topo de uma seção sem removê-lo
  const Package* peekAtSection(int destinationId) const;

  //@ Verifica se a seção para um destino está vazia
  bool isSectionEmpty(int destinationId) const;
};
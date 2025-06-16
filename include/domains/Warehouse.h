#pragma once

#include "dataStructures/Stack.h"

//* Forward declarations
class Package;
class Stack;

//@ Classe que implementa o domínio armazem
class Warehouse {
private:
  //@ Struct que implementa uma sessão de armazém (guardando a pilha de pacotes para um armazém específico )
  struct Session {
    //@ Atributo que guarda o ID do destino
    int destinationId;

    //@ Atributo que guarda a pilha de pacotes para o destino
    Stack stack;
  };

  //@ Atributo identificador do armazém
  int ID;

  //@ Pilhas de pacotes do armazém (escolha de array pois o acesso se torna mais
  // rapido)
  //! Sei que o máximo de pilhas vai ser o máximo de armazens no sistema
  Session *sessions;

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
  void storePackage(const Package *pkg, int destinationId);

  //@ Recupera (remove) um pacote da seção de um destino (operação principal)
  const Package *retrievePackage(int destinationId);

  //@ Espia o pacote no topo de uma seção sem removê-lo
  const Package *peekAtSection(int destinationId) const;

  //@ Verifica se a seção para um destino está vazia
  bool isSectionEmpty(int destinationId) const;
};
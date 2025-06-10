#pragma once

#include "Stack.h"

//@ Classe que implementa o domínio armazem
class Warehouse {
private:
  //@ Struct que define uma sessão de empilhamento de pacotes
  struct Session {
    //@ Pilha de pacotes
    Stack packages;

    //@ Armazem relacionado àquela pilha
    int relatedWarehouseId;
  };

  //@ Atributo identificador do armazém
  int ID;

  //@ Coleção de sessões de empilhamento de pacotes
  LinkedList<Session> sessions;

public:
  //@ Construtor
  //@ @param ID Identificador único do armazém
  // TODO: PASSAR AQUI A CAPACIDADE DO ARMAZEM QUE PASSA PARA O CONSTRUTOR DA
  // PILHA INTERNO ( PONTO EXTRA)
  Warehouse(int ID);

  //@ Função que retorna o ID do armazém
  int getId() const noexcept;

  //@ Função que retorna a pilha de pacotes do armazém
  Stack getPackages() const noexcept;
};
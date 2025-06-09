#pragma once

#include "Stack.h"

class Warehouse {
private:
  //@ Atributo identificador do armazém
  int ID;

  //@ Atributo que armazena os pacotes no armazém na pilha
  Stack packages;

public:
    //@ Construtor
    //@ @param ID Identificador único do armazém
    //TODO: PASSAR AQUI A CAPACIDADE DO ARMAZEM QUE PASSA PARA O CONSTRUTOR DA PILHA INTERNO ( PONTO EXTRA)
    Warehouse(int ID);

    //@ Função que retorna o ID do armazém
    int getId() const noexcept;

    //@ Função que retorna a pilha de pacotes do armazém
    Stack getPackages() const noexcept;
};
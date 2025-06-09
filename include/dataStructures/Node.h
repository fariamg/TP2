#pragma once

#include "Package.h"

//@ Struct de implemtação de um nó
struct Node {
  //@ Ponteiro para o pacote armazenado no nó
  Package *package;

  //@ Ponteiro para o próximo nó na pilha
  Node *next;
};
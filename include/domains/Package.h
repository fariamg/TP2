#pragma once

#include "LinkedList.h"
#include "PackageState.h"
#include "Warehouse.h"
#include <string>

//@ Classe que representa um pacote
class Package {
private:
  // TODO: type vai ser de um tipo específic
  //@ Atributo que guarda o ID único do pacote
  int ID;

  //@ Atributos que guardam o remetente, destinatário e tipo do pacote
  // std::string sender, recipient;

  //@ Atributos que guardam o armazém de origem e destino do pacote
  int initialOrigin, finalDestination;

  //@ Atributo que guarda a data de postagem do pacote
  int postDate;

  //@ Atributos que guardam o tempo que o pacote foi armazenado, o tempo em
  //@ trânsito e o tempo esperado de armazenamento
  int timeStored, timeInTransit, expectedStorageTime;

  //@ Atributo que guarda o estado do pacote (começa como NOT_POSTED)
  PackageState state;

  //@ Atributo que guarda a rota ótima entre armazéns para o pacote
  LinkedList<Warehouse> route;

public:
  //@ Construtor padrão
  //@ @param ID ID único do pacote
  Package(int ID, int initialOrigin, int finalDestination, int postDate,
          int expectedStorageTime);

  //@ Função que retorna o ID do pacote
  int getId() const noexcept;

  //@ Função que retorna o remetente do pacote
  // std::string getSender() const noexcept;

  //@ Função que retorna o destinatário do pacote'
  // std::string getRecipient() const noexcept;

  //@ Função que retorna o tipo do pacote
  std::string getType() const noexcept;

  //@ Função que retorna o armazém de origem do pacote
  Warehouse getInitialOrigin() const noexcept;

  //@ Função que retorna o armazém de destino do pacote
  Warehouse getFinalDestination() const noexcept;

  //@ Função que retorna a data de postagem do pacote
  int getPostDate() const noexcept;

  //@ Função que retorna o tempo armazenado do pacote
  int getTimeStored() const noexcept;

  //@ Função que retorna o tempo em trânsito do pacote
  int getTimeInTransit() const noexcept;

  //@ Função que retorna o tempo esperado de armazenamento do pacote
  int getExpectedStorageTime() const noexcept;

  //@ Função que retorna o estado do pacote
  PackageState getState() const noexcept;

  //@ Função que retorna a rota ótima entre armazéns para o pacote
  LinkedList<Warehouse> getRoute() const noexcept;

  //@ Função que seta o tempo que o pacote ficou em trânsito
  //@ @param time Tempo que o pacote ficou em trânsito
  void setTimeInTransit(int time);

  //@ Função que seta o tempo de armazenamento do pacote
  //@ @param time Tempo que o pacote ficou armazenado
  void setTimeStored(int time);
};
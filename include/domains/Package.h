#pragma once

#include "dataStructures/LinkedList.h"
#include "dataStructures/Graph.h"
#include <string>

class Graph;

//@ Enum que define os estados possíveis de um pacote
enum PackageState {
    NOT_POSTED,
    SCHEDULED,
    STORED,
    REMOVED_FOR_TRANSPORT,
    DELIVERED,
};
//@ Classe que representa um pacote
class Package {
  private:
    //@ Atributo que guarda o ID único do pacote
    int ID;

    //@ Atributos que guardam o armazém de origem e destino do pacote
    int initialOrigin, finalDestination;

    //@ Atributo que guarda a data de postagem do pacote
    int postTime;

    //@ Atributos que guardam o tempo que o pacote foi armazenado, o tempo em
    //@ trânsito e o tempo esperado de armazenamento
    int timeStored, timeInTransit;

    //@ Atributo que guarda a localização atual do pacote (começa como o armazém
    //@ origem)
    int currentLocation;

    //@ Atributo que guarda o estado do pacote (começa como NOT_POSTED)
    PackageState state;

    //@ Atributo que guarda a rota ótima entre armazéns para o pacote
    LinkedList route;

  public:
    //@ Construtor padrão
    //@ @param ID ID único do pacote
    Package(int ID, int initialOrigin, int finalDestination, int postTime);

    //@ Função que retorna o ID do pacote
    int getId() const noexcept;

    //@ Função que retorna o tipo do pacote
    std::string getType() const noexcept;

    //@ Função que retorna o armazém de origem do pacote
    int getInitialOrigin() const noexcept;

    //@ Função que retorna o armazém de destino do pacote
    int getFinalDestination() const noexcept;

    //@ Função que retorna a data de postagem do pacote
    int getPostTime() const noexcept;

    //@ Função que retorna o tempo armazenado do pacote
    int getTimeStored() const noexcept;

    //@ Função que retorna o tempo em trânsito do pacote
    int getTimeInTransit() const noexcept;

    //@ Função que retorna a localização atual do pacote
    int getCurrentLocation() const noexcept;

    //@ Função que retorna o estado do pacote
    PackageState getState() const noexcept;

    //@ Função que retorna a rota ótima entre armazéns para o pacote
    LinkedList& getRoute() noexcept;

    //@ Função que seta o tempo que o pacote ficou em trânsito
    //@ @param time Tempo que o pacote ficou em trânsito
    void setTimeInTransit(int time);

    //@ Função que seta a localização atual do pacote
    //@ @param location ID do armazém onde o pacote está localizado
    void setCurrentLocation(int location);

    //@ Função que seta o tempo de armazenamento do pacote
    //@ @param time Tempo que o pacote ficou armazenado
    void setTimeStored(int time);

    //@ Função que seta a melhor rota entre armazéns para o pacote
    void setRoute(LinkedList&& newRoute) noexcept;

    //@ Função que seta o estado do pacote
    void setState(PackageState newState) noexcept;
};


namespace Routing {

//@ Função que calcula a rota ótima entre dois armazéns
//@ @param originID ID do armazém de origem
//@ @param destinationID ID do armazém de destino
//@ @param graph Grafo que representa os armazéns e suas conexões
LinkedList calculateOptimalRoute(int origninID, int destinationID, const Graph& graph);
} // namespace Routing
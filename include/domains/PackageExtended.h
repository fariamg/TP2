#pragma once

#include "../dataStructures/LinkedList.h"
#include "Package.h"

//@ Enum que define os tipos de pacotes por peso
enum PackageWeightClass {
    LIGHT = 1,       // 1kg
    MEDIUM = 3,      // 3kg
    HEAVY = 5,       // 5kg
    EXTRA_HEAVY = 10 // 10kg
};

//@ Classe que representa um pacote com peso variável
class PackageExtended : public Package {
  private:
    //@ Peso do pacote
    int weight;

    //@ Classe de peso do pacote
    PackageWeightClass weightClass;

    //@ Histórico de rotas utilizadas
    LinkedList routeHistory;

    //@ Tempo de roteamento dinâmico
    int lastRouteCalculation;

    //@ Indicador se o pacote requer roteamento especial
    bool requiresSpecialHandling;

  public:
    //@ Construtor estendido
    PackageExtended(int ID, int initialOrigin, int finalDestination, int postTime, int weight = 1, bool specialHandling = false);

    //@ Função que retorna o peso do pacote
    int getWeight() const noexcept;

    //@ Função que retorna a classe de peso do pacote
    PackageWeightClass getWeightClass() const noexcept;

    //@ Função que retorna se requer manuseio especial
    bool getRequiresSpecialHandling() const noexcept;

    //@ Função que retorna o histórico de rotas
    const LinkedList& getRouteHistory() const noexcept;

    //@ Função que retorna o tempo da última calculação de rota
    int getLastRouteCalculation() const noexcept;

    //@ Função que adiciona uma rota ao histórico
    void addRouteToHistory(const LinkedList& route);

    //@ Função que atualiza o tempo da última calculação de rota
    void updateLastRouteCalculation(int time);

    //@ Função que calcula o impacto do peso no transporte
    double getWeightImpactFactor() const noexcept;

    //@ Função que calcula o impacto do peso no armazenamento
    int getStorageSpaceRequired() const noexcept;

  private:
    //@ Função para determinar a classe de peso baseada no peso
    PackageWeightClass determineWeightClass(int weight);
};
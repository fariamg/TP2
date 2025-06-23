#pragma once

#include "../dataStructures/Queue.h"
#include "../dataStructures/Stack.h"
#include "Warehouse.h"

//@ Forward declaration
class PackageExtended;

//@ Estrutura para representar uma seção de armazém com capacidade limitada
struct SectionExtended {
    int destinationId;
    Stack stack;
    int maxCapacity;
    int currentOccupancy;
    int weightCapacity;
    int currentWeight;
    Queue waitingQueue; // Fila para pacotes esperando espaço

    SectionExtended() : destinationId(-1), maxCapacity(100), currentOccupancy(0), weightCapacity(500), currentWeight(0) {}
};

//@ Classe que implementa o armazém estendido com capacidade limitada
class WarehouseExtended {
  private:
    //@ Atributo identificador do armazém
    int ID;

    //@ Seções de armazém com capacidade limitada
    SectionExtended* sections;

    //@ Total de armazéns no sistema
    int totalWarehouses;

    //@ Capacidade total do armazém
    int totalCapacity;

    //@ Ocupação atual total
    int totalOccupancy;

    //@ Capacidade de peso total
    int totalWeightCapacity;

    //@ Peso atual total
    int totalCurrentWeight;

    //@ Armazém secundário para overflow (ID do armazém)
    int secondaryStorageId;

    //@ Indicador se tem armazenamento secundário
    bool hasSecondaryStorage;

  public:
    //@ Construtor
    WarehouseExtended(int ID, int totalWarehouses, int capacity = 1000, int weightCapacity = 5000);

    //@ Destrutor
    ~WarehouseExtended();

    //@ Retorna o ID do armazém
    int getId() const noexcept;

    //@ Retorna a capacidade total
    int getTotalCapacity() const noexcept;

    //@ Retorna a ocupação atual
    int getTotalOccupancy() const noexcept;

    //@ Retorna a capacidade de peso
    int getTotalWeightCapacity() const noexcept;

    //@ Retorna o peso atual
    int getTotalCurrentWeight() const noexcept;

    //@ Retorna a utilização percentual
    double getUtilizationPercentage() const noexcept;

    //@ Retorna o tamanho de uma seção específica
    int getSectionSize(int sectionId) const noexcept;

    //@ Retorna a capacidade disponível de uma seção
    int getSectionAvailableCapacity(int sectionId) const noexcept;

    //@ Retorna a capacidade de peso disponível de uma seção
    int getSectionAvailableWeight(int sectionId) const noexcept;

    //@ Verifica se pode armazenar um pacote
    bool canStorePackage(PackageExtended* pkg, int destinationId) const;

    //@ Armazena um pacote considerando capacidade limitada
    bool storePackage(PackageExtended* pkg, int destinationId);

    //@ Tenta armazenar na fila de espera
    bool storeInWaitingQueue(PackageExtended* pkg, int destinationId);

    //@ Processa fila de espera quando há espaço disponível
    void processWaitingQueue(int destinationId);

    //@ Recupera (remove) um pacote da seção de um destino
    PackageExtended* retrievePackage(int destinationId);

    //@ Espia o pacote no topo de uma seção sem removê-lo
    const PackageExtended* peekAtSection(int destinationId) const;

    //@ Verifica se a seção para um destino está vazia
    bool isSectionEmpty(int destinationId) const;

    //@ Verifica se a seção está sobre capacidade
    bool isSectionOverCapacity(int destinationId) const;

    //@ Configura armazenamento secundário
    void setSecondaryStorage(int warehouseId);

    //@ Transfere pacote para armazenamento secundário
    bool transferToSecondaryStorage(PackageExtended* pkg, int destinationId, WarehouseExtended** warehouses);

    //@ Retorna o tamanho da fila de espera
    int getWaitingQueueSize(int destinationId) const noexcept;

    //@ Função para otimizar capacidade (realoca pacotes se necessário)
    void optimizeCapacity();

  private:
    //@ Log para pacote esperando devido à capacidade
    void logPackageWaitingDueToCapacity(int time, int pkgId, int whId, int sectionId);
};
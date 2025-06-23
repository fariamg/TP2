#pragma once

#include "../dataStructures/Graph.h"
#include "ConfigData.h"

//@ Forward declarations
class WarehouseExtended;
class PackageExtended;
class GraphExtended;

//@ Estrutura que guarda os dados de configuração estendidos com pontos extras
struct ConfigDataExtended {
    // Parâmetros base
    int transportCapacity;
    int transportLatency;
    int transportInterval;
    int removalCost;
    int numWarehouses;
    int numPackages;

    // Novos parâmetros para pontos extras
    bool variableTransportTime;
    bool variablePackageWeight;
    bool variableTransportCapacity;
    bool limitedStorageCapacity;
    bool multipleRoutes;
    bool dynamicRouting;

    // Estruturas estendidas
    GraphExtended* graph;
    WarehouseExtended** warehouses;
    PackageExtended** packages;

    //@ Construtor padrão
    ConfigDataExtended();
};

//@ Funções para carregar e limpar dados de configuração estendidos
ConfigDataExtended loadInputExtended(const std::string& filename);
void cleanupExtended(ConfigDataExtended& data);
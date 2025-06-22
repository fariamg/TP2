#pragma once

#include "../dataStructures/Graph.h"

//@ Forward declarations
class Warehouse;
class Package;

//@ Estrutura que guarda os dados de configuração lidos do arquivo de entrada
struct ConfigData {
    int transportCapacity;
    int transportLatency;
    int transportInterval;
    int removalCost;
    int numWarehouses;
    int numPackages;
    Graph* graph;
    Warehouse** warehouses;
    Package** packages;

    //@ Construtor padrão
    ConfigData();
};

//@ Funções para carregar e limpar dados de configuração
ConfigData loadInput(const std::string& filename);
void cleanup(ConfigData& data);
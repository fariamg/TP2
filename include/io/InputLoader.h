#pragma once

#include "../include/dataStructures/Graph.h"
#include "../include/domains/Package.h"

//@ Estrutura responsável por interagir com o arquivo de entrada
namespace IO {
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
};

//@ Função que lê as configurações gerais do arquivo de entrada
ConfigData loadInput(const std::string& filePath);

//@ Função que libera a memória alocada para os dados de configuração
void cleanup(ConfigData& data);
}; // namespace IO
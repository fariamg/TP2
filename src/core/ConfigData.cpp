#include "../../include/core/ConfigData.h"
#include "../../include/domains/Package.h"
#include "../../include/domains/Warehouse.h"
#include <fstream>
#include <stdexcept>
#include <string>

ConfigData::ConfigData() : transportCapacity(0), transportLatency(0), transportInterval(0), removalCost(0), numWarehouses(0), numPackages(0), graph(nullptr), warehouses(nullptr), packages(nullptr) {}

//@ Funções auxiliares para leitura do arquivo de entrada
void readGeneralConfig(std::ifstream& file, ConfigData& data) {
    file >> data.transportCapacity >> data.transportLatency >> data.transportInterval >> data.removalCost;
    if (file.fail()) {
        throw std::runtime_error("Erro ao ler configuracoes gerais do arquivo.");
    }
}

void readWarehouseAndGraphData(std::ifstream& file, ConfigData& data) {
    file >> data.numWarehouses;
    if (file.fail() || data.numWarehouses <= 0) {
        throw std::runtime_error("Erro ao ler numero de armazens ou numero invalido.");
    }

    data.graph = new Graph(data.numWarehouses);
    data.warehouses = new Warehouse*[data.numWarehouses];
    for (int i = 0; i < data.numWarehouses; ++i) {
        data.warehouses[i] = new Warehouse(i, data.numWarehouses);
    }

    for (int i = 0; i < data.numWarehouses; ++i) {
        for (int j = 0; j < data.numWarehouses; ++j) {
            int connection;
            file >> connection;
            if (file.fail()) {
                throw std::runtime_error("Erro ao ler matriz de adjacencia dos armazens.");
            }
            if (connection == 1 && i < j) {
                data.graph->addEdge(i, j, data.transportCapacity);
            }
        }
    }
}

void readPackageData(std::ifstream& file, ConfigData& data) {
    file >> data.numPackages;
    if (file.fail() || data.numPackages < 0) {
        throw std::runtime_error("Erro ao ler numero de pacotes ou numero invalido.");
    }
    if (data.numPackages == 0) {
        data.packages = nullptr;
        return;
    }

    data.packages = new Package*[data.numPackages];
    std::string trash;

    for (int i = 0; i < data.numPackages; ++i) {
        int postTime, origin, destination;
        file >> postTime >> trash >> trash >> trash >> origin >> trash >> destination;
        if (file.fail()) {
            throw std::runtime_error("Erro ao ler dados do pacote " + std::to_string(i));
        }
        if (origin < 0 || origin >= data.numWarehouses || destination < 0 || destination >= data.numWarehouses) {
            throw std::runtime_error("Origem ou destino invalido para o pacote " + std::to_string(i));
        }

        data.packages[i] = new Package(i, origin, destination, postTime);
    }
}

ConfigData loadInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de entrada: " + filename);
    }

    ConfigData data;

    try {
        readGeneralConfig(file, data);
        readWarehouseAndGraphData(file, data);
        readPackageData(file, data);
    } catch (const std::exception& e) {
        file.close();
        throw;
    }

    file.close();
    return data;
}

void cleanup(ConfigData& data) {
    if (data.graph) {
        delete data.graph;
        data.graph = nullptr;
    }
    if (data.warehouses) {
        for (int i = 0; i < data.numWarehouses; ++i) {
            delete data.warehouses[i];
        }
        delete[] data.warehouses;
        data.warehouses = nullptr;
    }
    if (data.packages) {
        for (int i = 0; i < data.numPackages; ++i) {
            delete data.packages[i];
        }
        delete[] data.packages;
        data.packages = nullptr;
    }
    data.transportCapacity = 0;
    data.transportLatency = 0;
    data.transportInterval = 0;
    data.removalCost = 0;
    data.numWarehouses = 0;
    data.numPackages = 0;
}
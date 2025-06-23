#include "../../include/core/ConfigDataExtended.h"
#include "../../include/dataStructures/GraphExtended.h"
#include "../../include/domains/PackageExtended.h"
#include "../../include/domains/WarehouseExtended.h"
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

ConfigDataExtended::ConfigDataExtended()
    : transportCapacity(0), transportLatency(0), transportInterval(0), removalCost(0), numWarehouses(0), numPackages(0), variableTransportTime(false), variablePackageWeight(false),
      variableTransportCapacity(false), limitedStorageCapacity(false), multipleRoutes(false), dynamicRouting(false), graph(nullptr), warehouses(nullptr), packages(nullptr) {}

ConfigDataExtended loadInputExtended(const std::string& filename) {
    ConfigDataExtended config;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
        return config;
    }

    std::string line;

    // Lê parâmetros base
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> config.transportCapacity >> config.transportLatency >> config.transportInterval >> config.removalCost;
    }

    // Lê parâmetros estendidos (nova linha no arquivo)
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        int flags;
        iss >> flags;

        config.variableTransportTime = (flags & 1) != 0;
        config.variablePackageWeight = (flags & 2) != 0;
        config.variableTransportCapacity = (flags & 4) != 0;
        config.limitedStorageCapacity = (flags & 8) != 0;
        config.multipleRoutes = (flags & 16) != 0;
        config.dynamicRouting = (flags & 32) != 0;
    }

    // Lê número de armazéns
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> config.numWarehouses;
    }

    // Cria grafo estendido
    config.graph = new GraphExtended(config.numWarehouses);

    // Lê arestas do grafo
    std::string graphLine;
    while (std::getline(file, graphLine) && !graphLine.empty()) {
        std::istringstream iss(graphLine);
        int from, to, capacity, latency;
        double timeMultiplier = 1.0;

        iss >> from >> to >> capacity;

        if (config.variableTransportTime) {
            iss >> latency >> timeMultiplier;
        } else {
            latency = config.transportLatency;
        }

        config.graph->addEdge(from, to, capacity, latency, timeMultiplier);
    }

    // Cria armazéns estendidos
    config.warehouses = new WarehouseExtended*[config.numWarehouses];
    for (int i = 0; i < config.numWarehouses; ++i) {
        int capacity = config.limitedStorageCapacity ? 1000 : 10000; // Capacidade padrão
        int weightCapacity = config.limitedStorageCapacity ? 5000 : 50000;
        config.warehouses[i] = new WarehouseExtended(i, config.numWarehouses, capacity, weightCapacity);
    }

    // Lê número de pacotes
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> config.numPackages;
    }

    // Cria pacotes estendidos
    config.packages = new PackageExtended*[config.numPackages];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> weightDist(1, 10);
    std::uniform_real_distribution<> specialDist(0.0, 1.0);

    for (int i = 0; i < config.numPackages; ++i) {
        if (std::getline(file, line)) {
            std::istringstream iss(line);
            int id, origin, destination, postTime;
            iss >> id >> origin >> destination >> postTime;

            int weight = config.variablePackageWeight ? weightDist(gen) : 1;
            bool specialHandling = specialDist(gen) < 0.1; // 10% dos pacotes precisam manuseio especial

            config.packages[i] = new PackageExtended(id, origin, destination, postTime, weight, specialHandling);

            // Calcula rota inicial
            Route initialRoute = config.graph->findBestRoute(origin, destination, weight, 0);
            if (initialRoute.path.getCurrentSize() > 0) {
                config.packages[i]->setRoute(std::move(initialRoute.path));
            }
        }
    }

    file.close();
    return config;
}

void cleanupExtended(ConfigDataExtended& data) {
    // Limpa pacotes
    if (data.packages) {
        for (int i = 0; i < data.numPackages; ++i) {
            delete data.packages[i];
        }
        delete[] data.packages;
        data.packages = nullptr;
    }

    // Limpa armazéns
    if (data.warehouses) {
        for (int i = 0; i < data.numWarehouses; ++i) {
            delete data.warehouses[i];
        }
        delete[] data.warehouses;
        data.warehouses = nullptr;
    }

    // Limpa grafo
    delete data.graph;
    data.graph = nullptr;
}
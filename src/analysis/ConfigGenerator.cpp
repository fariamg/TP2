#include "../../include/analysis/ConfigGenerator.h"
#include <algorithm>
#include <fstream>
#include <random>
#include <set>

std::vector<ConfigGenerator::TestConfig> ConfigGenerator::generateWarehouseVariations(int minWarehouses, int maxWarehouses, int step, int basePackages, int baseCapacity, int baseLatency,
                                                                                      int baseInterval) {

    std::vector<TestConfig> configs;

    for (int warehouses = minWarehouses; warehouses <= maxWarehouses; warehouses += step) {
        TestConfig config;
        config.numWarehouses = warehouses;
        config.numPackages = basePackages; // FIXO para isolar variável de teste
        config.transportCapacity = baseCapacity;
        config.transportLatency = baseLatency;
        config.transportInterval = baseInterval;
        config.removalCost = 1;
        config.description = "warehouses_" + std::to_string(warehouses);

        configs.push_back(config);
    }

    return configs;
}

std::vector<ConfigGenerator::TestConfig> ConfigGenerator::generatePackageVariations(int minPackages, int maxPackages, int step, int baseWarehouses, int baseCapacity, int baseLatency,
                                                                                    int baseInterval) {

    std::vector<TestConfig> configs;

    for (int packages = minPackages; packages <= maxPackages; packages += step) {
        TestConfig config;
        config.numWarehouses = baseWarehouses; // FIXO para isolar variável de teste
        config.numPackages = packages;
        config.transportCapacity = baseCapacity;
        config.transportLatency = baseLatency;
        config.transportInterval = baseInterval;
        config.removalCost = 1;
        config.description = "packages_" + std::to_string(packages);

        configs.push_back(config);
    }

    return configs;
}

std::vector<ConfigGenerator::TestConfig> ConfigGenerator::generateTransportContentionVariations(int baseWarehouses, int basePackages) {

    std::vector<TestConfig> configs;

    // Configurações que variam contenção de forma mais agressiva
    std::vector<std::tuple<int, int, int, std::string>> contentionConfigs = {
        {1, 5, 100, "maxima_contencao"},                                                                                                   // Mínima capacidade, máximo intervalo
        {2, 10, 80, "alta_contencao"},   {3, 15, 60, "media_contencao"}, {5, 20, 40, "baixa_contencao"}, {10, 25, 20, "minima_contencao"}, // Máxima capacidade, mínimo intervalo
    };

    for (const auto& [capacity, latency, interval, desc] : contentionConfigs) {
        TestConfig config;
        config.numWarehouses = baseWarehouses;
        config.numPackages = basePackages;
        config.transportCapacity = capacity;
        config.transportLatency = latency;
        config.transportInterval = interval;
        config.removalCost = 1;
        config.description = desc;

        configs.push_back(config);
    }

    return configs;
}

bool ConfigGenerator::saveConfigToFile(const TestConfig& config, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Escreve parâmetros gerais
    file << config.transportCapacity << " " << config.transportLatency << " " << config.transportInterval << " " << config.removalCost << std::endl;

    // Escreve número de armazéns
    file << config.numWarehouses << std::endl;

    // Gera e escreve matriz de adjacência (totalmente conectada para consistência)
    auto graph = generateConnectedGraph(config.numWarehouses);
    for (int i = 0; i < config.numWarehouses; i++) {
        for (int j = 0; j < config.numWarehouses; j++) {
            file << graph[i][j];
            if (j < config.numWarehouses - 1)
                file << " ";
        }
        file << std::endl;
    }

    // Gera e escreve pacotes com tempos determinísticos
    auto packages = generateDeterministicPackages(config.numPackages, config.numWarehouses);
    file << config.numPackages << std::endl;

    for (const auto& [time, id, origin, dest] : packages) {
        file << time << " pac " << id << " org " << origin << " dst " << dest << std::endl;
    }

    file.close();
    return true;
}

std::vector<std::vector<int>> ConfigGenerator::generateConnectedGraph(int numWarehouses) {
    std::vector<std::vector<int>> graph(numWarehouses, std::vector<int>(numWarehouses, 0));

    // Cria grafo totalmente conectado para consistência
    for (int i = 0; i < numWarehouses; i++) {
        for (int j = i + 1; j < numWarehouses; j++) {
            graph[i][j] = 1;
            graph[j][i] = 1;
        }
    }

    return graph;
}

std::vector<std::tuple<int, int, int, int>> ConfigGenerator::generateDeterministicPackages(int numPackages, int numWarehouses) {

    std::vector<std::tuple<int, int, int, int>> packages;

    // Seed fixo para resultados reproduzíveis
    std::mt19937 gen(12345);
    std::uniform_int_distribution<> warehouseDis(0, numWarehouses - 1);

    for (int i = 0; i < numPackages; i++) {
        // Tempo de chegada espaçado uniformemente para evitar picos
        int time = (i * 10) + 1; // Chegada a cada 10 unidades de tempo

        int origin = warehouseDis(gen);
        int dest = warehouseDis(gen);

        // Garante que origem e destino são diferentes
        while (dest == origin) {
            dest = warehouseDis(gen);
        }

        packages.emplace_back(time, i, origin, dest);
    }

    return packages;
}
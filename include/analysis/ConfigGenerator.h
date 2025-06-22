#pragma once

#include <string>
#include <tuple>
#include <vector>

//@ Classe responsável por gerar configurações de teste para análise experimental
class ConfigGenerator {
  public:
    //@ Estrutura que define os parâmetros de uma configuração de teste
    struct TestConfig {
        int transportCapacity;
        int transportLatency;
        int transportInterval;
        int removalCost;
        int numWarehouses;
        int numPackages;
        std::string description;
    };

    //@ Gera configurações variando o número de armazéns
    static std::vector<TestConfig> generateWarehouseVariations(int minWarehouses, int maxWarehouses, int step, int basePackages, int baseCapacity, int baseLatency, int baseInterval);

    //@ Gera configurações variando o número de pacotes
    static std::vector<TestConfig> generatePackageVariations(int minPackages, int maxPackages, int step, int baseWarehouses, int baseCapacity, int baseLatency, int baseInterval);

    //@ Gera configurações variando a contenção por transporte
    static std::vector<TestConfig> generateTransportContentionVariations(int baseWarehouses, int basePackages);

    //@ Salva uma configuração específica como arquivo de entrada
    static bool saveConfigToFile(const TestConfig& config, const std::string& filename);

  private:
    //@ Gera um grafo conectado aleatório para os armazéns
    static std::vector<std::vector<int>> generateConnectedGraph(int numWarehouses);

    //@ Gera pacotes determinísticos para teste
    static std::vector<std::tuple<int, int, int, int>> generateDeterministicPackages(int numPackages, int numWarehouses);
};
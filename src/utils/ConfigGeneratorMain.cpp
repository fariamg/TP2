#include "../include/analysis/ConfigGenerator.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Gerando arquivos de configuração de teste..." << std::endl;

    // Generate base test configuration
    ConfigGenerator::TestConfig baseConfig;
    baseConfig.transportCapacity = 2;
    baseConfig.transportLatency = 10;
    baseConfig.transportInterval = 5;
    baseConfig.removalCost = 1;
    baseConfig.numWarehouses = 5;
    baseConfig.numPackages = 20;
    baseConfig.description = "Base test configuration";

    bool success = ConfigGenerator::saveConfigToFile(baseConfig, "configs/test_config.txt");
    if (success) {
        std::cout << "✓ Arquivo configs/test_config.txt criado" << std::endl;
    } else {
        std::cerr << "✗ Erro ao criar configs/test_config.txt" << std::endl;
        return 1;
    }

    // Generate extended test configuration
    ConfigGenerator::TestConfig extendedConfig;
    extendedConfig.transportCapacity = 3;
    extendedConfig.transportLatency = 8;
    extendedConfig.transportInterval = 4;
    extendedConfig.removalCost = 1;
    extendedConfig.numWarehouses = 7;
    extendedConfig.numPackages = 30;
    extendedConfig.description = "Extended test configuration";

    success = ConfigGenerator::saveConfigToFile(extendedConfig, "configs/test_config_extended.txt");
    if (success) {
        std::cout << "✓ Arquivo configs/test_config_extended.txt criado" << std::endl;
    } else {
        std::cerr << "✗ Erro ao criar configs/test_config_extended.txt" << std::endl;
        return 1;
    }

    // Generate additional test configurations for experiments
    std::cout << "Gerando configurações de experimento..." << std::endl;

    // Small test
    ConfigGenerator::TestConfig smallConfig;
    smallConfig.transportCapacity = 1;
    smallConfig.transportLatency = 5;
    smallConfig.transportInterval = 3;
    smallConfig.removalCost = 1;
    smallConfig.numWarehouses = 3;
    smallConfig.numPackages = 10;
    smallConfig.description = "Small test";

    ConfigGenerator::saveConfigToFile(smallConfig, "configs/small_test.txt");

    // Medium test
    ConfigGenerator::TestConfig mediumConfig;
    mediumConfig.transportCapacity = 4;
    mediumConfig.transportLatency = 15;
    mediumConfig.transportInterval = 7;
    mediumConfig.removalCost = 2;
    mediumConfig.numWarehouses = 10;
    mediumConfig.numPackages = 50;
    mediumConfig.description = "Medium test";

    ConfigGenerator::saveConfigToFile(mediumConfig, "configs/medium_test.txt");

    // Large test
    ConfigGenerator::TestConfig largeConfig;
    largeConfig.transportCapacity = 5;
    largeConfig.transportLatency = 20;
    largeConfig.transportInterval = 10;
    largeConfig.removalCost = 3;
    largeConfig.numWarehouses = 15;
    largeConfig.numPackages = 100;
    largeConfig.description = "Large test";

    ConfigGenerator::saveConfigToFile(largeConfig, "configs/large_test.txt");

    std::cout << "✓ Configurações adicionais criadas:" << std::endl;
    std::cout << "  - configs/small_test.txt" << std::endl;
    std::cout << "  - configs/medium_test.txt" << std::endl;
    std::cout << "  - configs/large_test.txt" << std::endl;

    std::cout << "Configurações geradas com sucesso!" << std::endl;
    return 0;
}
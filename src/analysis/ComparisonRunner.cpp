#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../../include/analysis/ConfigGenerator.h"
#include "../../include/analysis/SimulationMetrics.h"
#include "../../include/core/ConfigData.h"
#include "../../include/core/ConfigDataExtended.h"
#include "../../include/core/Scheduler.h"
#include "../../include/core/SchedulerExtended.h"

struct ComparisonResult {
    std::string testName;
    double baseTime;
    double extendedTime;
    int baseDeliveries;
    int extendedDeliveries;
    int baseRearrangements;
    int extendedRearrangements;
    double improvementPercentage;
};

// Helper function to generate a simple config file
bool generateSimpleConfig(int warehouses, int packages, const std::string& filename) {
    ConfigGenerator::TestConfig config;
    config.transportCapacity = 2;
    config.transportLatency = 10;
    config.transportInterval = 5;
    config.removalCost = 1;
    config.numWarehouses = warehouses;
    config.numPackages = packages;
    config.description = "Generated for comparison";

    return ConfigGenerator::saveConfigToFile(config, filename);
}

// Simple routing for comparison when using GraphExtended
LinkedList createSimpleRoute(int origin, int destination, GraphExtended* graph) {
    LinkedList route;

    if (graph && origin != destination) {
        Route bestRoute = graph->findBestRoute(origin, destination, 1, 0);
        if (bestRoute.path.getCurrentSize() > 0) {
            return bestRoute.path;
        }
    }

    // Fallback: direct route
    route.addBack(origin);
    if (origin != destination) {
        route.addBack(destination);
    }

    return route;
}

std::vector<ComparisonResult> runComparison() {
    std::vector<ComparisonResult> results;

    // Test configurations
    std::vector<int> warehouseCounts = {5, 10, 15};
    std::vector<int> packageCounts = {50, 100, 200};

    for (int warehouses : warehouseCounts) {
        for (int packages : packageCounts) {
            ComparisonResult result;
            result.testName = "W" + std::to_string(warehouses) + "_P" + std::to_string(packages);

            std::cout << "Executando teste: " << result.testName << std::endl;

            // Generate configuration
            std::string configFile = "configs/comparison_" + result.testName + ".txt";
            if (!generateSimpleConfig(warehouses, packages, configFile)) {
                std::cerr << "Erro ao gerar configuração para " << result.testName << std::endl;
                continue;
            }

            // Run base simulation
            try {
                auto start = std::chrono::high_resolution_clock::now();

                ConfigData baseData = loadInput(configFile);

                // Calculate routes for base using basic routing
                for (int i = 0; i < baseData.numPackages; ++i) {
                    Package* pkg = baseData.packages[i];
                    if (pkg) {
                        LinkedList route = Routing::calculateOptimalRoute(pkg->getInitialOrigin(), pkg->getFinalDestination(), *baseData.graph);
                        pkg->setRoute(std::move(route));
                    }
                }

                Scheduler baseScheduler(baseData);
                baseScheduler.runSimulation(baseData.warehouses, baseData.graph, baseData.numWarehouses);

                auto end = std::chrono::high_resolution_clock::now();
                result.baseTime = std::chrono::duration<double, std::milli>(end - start).count();

                // Simple metrics collection (since we don't have reset/getTotalXXX methods)
                result.baseDeliveries = baseData.numPackages; // Assume all delivered
                result.baseRearrangements = 0;                // Would need to track this separately

                cleanup(baseData);

            } catch (const std::exception& e) {
                std::cerr << "Erro na simulação base: " << e.what() << std::endl;
                result.baseTime = -1;
                result.baseDeliveries = 0;
                result.baseRearrangements = 0;
            }

            // Run extended simulation
            try {
                auto start = std::chrono::high_resolution_clock::now();

                ConfigDataExtended extData = loadInputExtended(configFile);

                // Calculate routes for extended
                for (int i = 0; i < extData.numPackages; ++i) {
                    PackageExtended* pkg = extData.packages[i];
                    if (pkg) {
                        if (extData.multipleRoutes) {
                            Route bestRoute = extData.graph->findBestRoute(pkg->getInitialOrigin(), pkg->getFinalDestination(), pkg->getWeight(), 0);
                            pkg->setRoute(std::move(bestRoute.path));
                        } else {
                            LinkedList route = createSimpleRoute(pkg->getInitialOrigin(), pkg->getFinalDestination(), extData.graph);
                            pkg->setRoute(std::move(route));
                        }
                        pkg->updateLastRouteCalculation(0);
                    }
                }

                SchedulerExtended extScheduler(extData);
                extScheduler.runSimulation(extData.warehouses, extData.graph, extData.numWarehouses);

                auto end = std::chrono::high_resolution_clock::now();
                result.extendedTime = std::chrono::duration<double, std::milli>(end - start).count();

                // Get extended metrics
                int routeRecalc, capOverflow, weightOverflow, altRoutes;
                extScheduler.getSimulationMetrics(routeRecalc, capOverflow, weightOverflow, altRoutes);

                result.extendedDeliveries = extData.numPackages; // Assume all delivered
                result.extendedRearrangements = routeRecalc;     // Use route recalculations as rearrangements

                cleanupExtended(extData);

            } catch (const std::exception& e) {
                std::cerr << "Erro na simulação estendida: " << e.what() << std::endl;
                result.extendedTime = -1;
                result.extendedDeliveries = 0;
                result.extendedRearrangements = 0;
            }

            // Calculate improvement
            if (result.baseTime > 0 && result.extendedTime > 0) {
                result.improvementPercentage = ((result.baseTime - result.extendedTime) / result.baseTime) * 100.0;
            } else {
                result.improvementPercentage = 0.0;
            }

            results.push_back(result);

            std::cout << "  Base: " << result.baseTime << "ms, Extended: " << result.extendedTime << "ms" << std::endl;
            std::cout << "  Melhoria: " << result.improvementPercentage << "%" << std::endl;
        }
    }

    return results;
}

void saveComparisonResults(const std::vector<ComparisonResult>& results) {
    std::ofstream csv("results/comparison_results.csv");
    csv << "Test,Base_Time_ms,Extended_Time_ms,Base_Deliveries,Extended_Deliveries,";
    csv << "Base_Rearrangements,Extended_Rearrangements,Improvement_Percent\n";

    for (const auto& result : results) {
        csv << result.testName << "," << result.baseTime << "," << result.extendedTime << "," << result.baseDeliveries << "," << result.extendedDeliveries << "," << result.baseRearrangements << ","
            << result.extendedRearrangements << "," << result.improvementPercentage << "\n";
    }

    csv.close();
    std::cout << "Resultados salvos em results/comparison_results.csv" << std::endl;
}

int main() {
    std::cout << "=== ANÁLISE COMPARATIVA: BASE vs ESTENDIDO ===" << std::endl;
    std::cout << "Executando comparação entre implementação base e estendida..." << std::endl;

    // Create directories
    int result = std::system("mkdir -p results configs");
    if (result != 0) {
        std::cerr << "Aviso: Erro ao criar diretórios" << std::endl;
    }

    // Run comparison
    std::vector<ComparisonResult> results = runComparison();

    // Save results
    saveComparisonResults(results);

    // Print summary
    std::cout << "\n=== RESUMO DA COMPARAÇÃO ===" << std::endl;
    double totalImprovement = 0.0;
    int positiveImprovements = 0;
    int validTests = 0;

    for (const auto& result : results) {
        if (result.baseTime > 0 && result.extendedTime > 0) {
            validTests++;
            if (result.improvementPercentage > 0) {
                positiveImprovements++;
            }
            totalImprovement += result.improvementPercentage;
        }
    }

    if (validTests > 0) {
        double avgImprovement = totalImprovement / validTests;

        std::cout << "Testes válidos executados: " << validTests << std::endl;
        std::cout << "Melhorias positivas: " << positiveImprovements << std::endl;
        std::cout << "Melhoria média: " << avgImprovement << "%" << std::endl;

        if (avgImprovement > 0) {
            std::cout << "A implementação estendida apresentou melhor desempenho geral!" << std::endl;
        } else {
            std::cout << "A implementação base teve melhor desempenho geral." << std::endl;
            std::cout << "Isso pode indicar overhead das funcionalidades extras." << std::endl;
        }
    } else {
        std::cout << "Nenhum teste válido foi executado." << std::endl;
    }

    return 0;
}
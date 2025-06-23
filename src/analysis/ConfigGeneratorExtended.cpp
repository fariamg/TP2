#include "../../include/analysis/ConfigGenerator.h"
#include <fstream>
#include <iostream>
#include <random>

class ConfigGeneratorExtended {
  private:
    std::mt19937 rng;

  public:
    ConfigGeneratorExtended() : rng(std::random_device{}()) {}

    void generateExtendedConfigurations() {
        generateVariableWeightConfigs();
        generateLimitedCapacityConfigs();
        generateMultipleRoutesConfigs();
        generateDynamicRoutingConfigs();
    }

  private:
    void generateVariableWeightConfigs() {
        std::ofstream file("data/extended_variable_weight.txt");

        // Parâmetros base
        file << "3\n";   // transportCapacity
        file << "25\n";  // transportLatency
        file << "120\n"; // transportInterval
        file << "2\n";   // removalCost
        file << "6\n";   // numWarehouses

        // Matriz de adjacência (topologia com ciclos para múltiplas rotas)
        file << "0 1 1 0 0 0\n";
        file << "1 0 1 1 0 0\n";
        file << "1 1 0 1 1 0\n";
        file << "0 1 1 0 1 1\n";
        file << "0 0 1 1 0 1\n";
        file << "0 0 0 1 1 0\n";

        // Pacotes com pesos variáveis
        file << "20\n"; // numPackages

        std::uniform_int_distribution<int> timeDist(1, 200);
        std::uniform_int_distribution<int> warehouseDist(0, 5);
        std::uniform_int_distribution<int> weightDist(1, 10);

        for (int i = 0; i < 20; ++i) {
            int time = timeDist(rng);
            int org = warehouseDist(rng);
            int dst = warehouseDist(rng);
            while (dst == org) dst = warehouseDist(rng);
            int weight = weightDist(rng);

            file << time << " pac " << i << " org " << org << " dst " << dst << " weight " << weight << "\n";
        }

        file.close();
    }

    void generateLimitedCapacityConfigs() {
        std::ofstream file("data/extended_limited_capacity.txt");

        file << "2\n";   // transportCapacity (reduzida)
        file << "30\n";  // transportLatency
        file << "100\n"; // transportInterval
        file << "3\n";   // removalCost
        file << "8\n";   // numWarehouses

        // Matriz de adjacência (árvore)
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if ((i == 0 && (j == 1 || j == 2)) || (i == 1 && (j == 0 || j == 3 || j == 4)) || (i == 2 && (j == 0 || j == 5 || j == 6)) || (i == 3 && j == 1) || (i == 4 && j == 1) ||
                    (i == 5 && j == 2) || (i == 6 && (j == 2 || j == 7)) || (i == 7 && j == 6)) {
                    file << "1 ";
                } else {
                    file << "0 ";
                }
            }
            file << "\n";
        }

        // Muitos pacotes para testar capacidade limitada
        file << "50\n";

        std::uniform_int_distribution<int> timeDist(1, 150);
        std::uniform_int_distribution<int> warehouseDist(0, 7);

        for (int i = 0; i < 50; ++i) {
            int time = timeDist(rng);
            int org = warehouseDist(rng);
            int dst = warehouseDist(rng);
            while (dst == org) dst = warehouseDist(rng);

            file << time << " pac " << i << " org " << org << " dst " << dst << "\n";
        }

        file.close();
    }

    void generateMultipleRoutesConfigs() {
        std::ofstream file("data/extended_multiple_routes.txt");

        file << "4\n";  // transportCapacity
        file << "20\n"; // transportLatency
        file << "90\n"; // transportInterval
        file << "1\n";  // removalCost
        file << "5\n";  // numWarehouses

        // Grafo completo para múltiplas rotas
        file << "0 1 1 1 1\n";
        file << "1 0 1 1 0\n";
        file << "1 1 0 1 1\n";
        file << "1 1 1 0 1\n";
        file << "1 0 1 1 0\n";

        file << "15\n"; // numPackages

        std::uniform_int_distribution<int> timeDist(1, 180);
        std::uniform_int_distribution<int> warehouseDist(0, 4);

        for (int i = 0; i < 15; ++i) {
            int time = timeDist(rng);
            int org = warehouseDist(rng);
            int dst = warehouseDist(rng);
            while (dst == org) dst = warehouseDist(rng);

            file << time << " pac " << i << " org " << org << " dst " << dst << "\n";
        }

        file.close();
    }

    void generateDynamicRoutingConfigs() {
        std::ofstream file("data/extended_dynamic_routing.txt");

        file << "3\n";   // transportCapacity
        file << "35\n";  // transportLatency
        file << "110\n"; // transportInterval
        file << "2\n";   // removalCost
        file << "7\n";   // numWarehouses

        // Topologia com múltiplos caminhos
        file << "0 1 1 0 0 0 0\n";
        file << "1 0 0 1 1 0 0\n";
        file << "1 0 0 0 1 1 0\n";
        file << "0 1 0 0 0 1 1\n";
        file << "0 1 1 0 0 0 1\n";
        file << "0 0 1 1 0 0 1\n";
        file << "0 0 0 1 1 1 0\n";

        file << "25\n"; // numPackages com chegadas espalhadas

        std::uniform_int_distribution<int> timeDist(1, 300);
        std::uniform_int_distribution<int> warehouseDist(0, 6);

        for (int i = 0; i < 25; ++i) {
            int time = timeDist(rng);
            int org = warehouseDist(rng);
            int dst = warehouseDist(rng);
            while (dst == org) dst = warehouseDist(rng);

            file << time << " pac " << i << " org " << org << " dst " << dst << "\n";
        }

        file.close();
    }
};
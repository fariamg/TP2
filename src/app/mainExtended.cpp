#include <iostream>
#include <stdexcept>
#include <string>

#include "../../include/core/ConfigDataExtended.h"
#include "../../include/core/SchedulerExtended.h"
#include "../../include/dataStructures/GraphExtended.h"
#include "../../include/domains/PackageExtended.h"
#include "../../include/domains/WarehouseExtended.h"
#include "../../include/utils/Logger.h"

//@ Função principal do programa estendido
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        std::cerr << "Simulação Estendida com Pontos Extras:" << std::endl;
        std::cerr << "- Tempo de transporte variável" << std::endl;
        std::cerr << "- Peso de pacote variável" << std::endl;
        std::cerr << "- Capacidade de transporte variável" << std::endl;
        std::cerr << "- Capacidade de armazenamento limitada" << std::endl;
        std::cerr << "- Múltiplas rotas entre armazéns" << std::endl;
        std::cerr << "- Geração dinâmica de rotas" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];
    ConfigDataExtended data = {};

    try {
        std::cout << "=== SIMULAÇÃO ESTENDIDA COM PONTOS EXTRAS ===" << std::endl;
        std::cout << "Carregando configuração estendida..." << std::endl;

        data = loadInputExtended(inputFilename);

        std::cout << "Configurações ativas:" << std::endl;
        std::cout << "- Tempo de transporte variável: " << (data.variableTransportTime ? "SIM" : "NÃO") << std::endl;
        std::cout << "- Peso de pacote variável: " << (data.variablePackageWeight ? "SIM" : "NÃO") << std::endl;
        std::cout << "- Capacidade de transporte variável: " << (data.variableTransportCapacity ? "SIM" : "NÃO") << std::endl;
        std::cout << "- Capacidade de armazenamento limitada: " << (data.limitedStorageCapacity ? "SIM" : "NÃO") << std::endl;
        std::cout << "- Múltiplas rotas: " << (data.multipleRoutes ? "SIM" : "NÃO") << std::endl;
        std::cout << "- Roteamento dinâmico: " << (data.dynamicRouting ? "SIM" : "NÃO") << std::endl;
        std::cout << std::endl;

        // Calcula rotas iniciais para todos os pacotes
        std::cout << "Calculando rotas iniciais..." << std::endl;
        for (int i = 0; i < data.numPackages; ++i) {
            PackageExtended* pkg = data.packages[i];
            if (pkg) {
                if (data.multipleRoutes) {
                    // Usa o algoritmo avançado de roteamento do GraphExtended
                    Route bestRoute = data.graph->findBestRoute(pkg->getInitialOrigin(), pkg->getFinalDestination(), pkg->getWeight(), 0);
                    pkg->setRoute(std::move(bestRoute.path));
                } else {
                    // Para compatibilidade, usa roteamento simples baseado na topologia
                    // Implementa um Dijkstra básico usando as estruturas do GraphExtended
                    LinkedList route;

                    // Adiciona origem e destino como rota direta para casos simples
                    route.addBack(pkg->getInitialOrigin());

                    // Se origem != destino, precisa encontrar caminho
                    if (pkg->getInitialOrigin() != pkg->getFinalDestination()) {
                        // Usa o método findBestRoute do GraphExtended mesmo sem múltiplas rotas
                        Route simpleRoute = data.graph->findBestRoute(pkg->getInitialOrigin(), pkg->getFinalDestination(),
                                                                      1, // peso padrão
                                                                      0  // tempo inicial
                        );

                        if (simpleRoute.path.getCurrentSize() > 0) {
                            route = simpleRoute.path;
                        } else {
                            // Fallback: adiciona destino diretamente se não encontrar rota
                            route.addBack(pkg->getFinalDestination());
                        }
                    }

                    pkg->setRoute(std::move(route));
                }
                pkg->updateLastRouteCalculation(0);
            }
        }

        std::cout << "Iniciando simulação estendida..." << std::endl;
        SchedulerExtended scheduler(data);
        scheduler.runSimulation(data.warehouses, data.graph, data.numWarehouses);

        // Exibe métricas finais
        std::cout << std::endl << "=== MÉTRICAS DA SIMULAÇÃO ESTENDIDA ===" << std::endl;
        int routeRecalc, capOverflow, weightOverflow, altRoutes;
        scheduler.getSimulationMetrics(routeRecalc, capOverflow, weightOverflow, altRoutes);

        std::cout << "Recálculos de rota: " << routeRecalc << std::endl;
        std::cout << "Overflows de capacidade: " << capOverflow << std::endl;
        std::cout << "Overflows de peso: " << weightOverflow << std::endl;
        std::cout << "Rotas alternativas usadas: " << altRoutes << std::endl;

        // Calcula métricas de eficiência
        if (data.limitedStorageCapacity) {
            double totalUtilization = 0.0;
            for (int i = 0; i < data.numWarehouses; ++i) {
                totalUtilization += data.warehouses[i]->getUtilizationPercentage();
            }
            double avgUtilization = totalUtilization / data.numWarehouses;
            std::cout << "Utilização média dos armazéns: " << avgUtilization << "%" << std::endl;
        }

        std::cout << "Simulação concluída com sucesso!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ocorreu um erro fatal: " << e.what() << std::endl;
        cleanupExtended(data);
        return 1;
    }

    cleanupExtended(data);
    return 0;
}
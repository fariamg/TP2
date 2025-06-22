#include <iostream>
#include <stdexcept>
#include <string>

#include "../include/core/RouteCalculator.h"
#include "../include/core/Scheduler.h"
#include "../include/io/InputLoader.h"

// Garanta que você tenha essa função de limpeza no seu InputLoader.cpp
namespace IO {
void cleanup(ConfigData& data);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];
    IO::ConfigData data = {}; // Inicializa a struct

    try {
        // ETAPA 1: Carrega todos os dados do arquivo.
        data = IO::loadInput(inputFilename);

        // ETAPA 2: CALCULAR AS ROTAS (ESTE PASSO É OBRIGATÓRIO)
        for (int i = 0; i < data.numPackages; ++i) {
            Package* pkg = data.packages[i];
            if (pkg) {
                LinkedList route = Routing::calculateOptimalRoute(pkg->getInitialOrigin(), pkg->getFinalDestination(), *data.graph);
                pkg->setRoute(std::move(route));
            }
        }

        // ETAPA 3: Cria e executa a simulação.
        Scheduler scheduler(data);
        scheduler.runSimulation(data.warehouses, data.graph, data.numWarehouses);

    } catch (const std::exception& e) {
        std::cerr << "Ocorreu um erro fatal: " << e.what() << std::endl;
        IO::cleanup(data); // Garante a limpeza mesmo em caso de erro.
        return 1;
    }

    // ETAPA 4: Limpeza da memória.
    IO::cleanup(data);
    return 0;
}
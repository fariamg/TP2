#include <iostream>
#include <stdexcept>
#include <string>

#include "../../include/core/ConfigData.h"
#include "../../include/core/Scheduler.h"
#include "../../include/dataStructures/Graph.h"
#include "../../include/domains/Package.h"
#include "../../include/domains/Warehouse.h"
#include "../../include/utils/Logger.h"

//@ Função principal do programa
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];
    ConfigData data = {};

    try {
        data = loadInput(inputFilename);

        for (int i = 0; i < data.numPackages; ++i) {
            Package* pkg = data.packages[i];
            if (pkg) {
                LinkedList route = Routing::calculateOptimalRoute(pkg->getInitialOrigin(), pkg->getFinalDestination(), *data.graph);
                pkg->setRoute(std::move(route));
            }
        }

        Scheduler scheduler(data);
        scheduler.runSimulation(data.warehouses, data.graph, data.numWarehouses);
    } catch (const std::exception& e) {
        std::cerr << "Ocorreu um erro fatal: " << e.what() << std::endl;
        cleanup(data);
        return 1;
    }

    cleanup(data);
    return 0;
}
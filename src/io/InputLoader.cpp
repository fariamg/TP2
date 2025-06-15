#include "../include/io/InputLoader.h"
#include "domains/Warehouse.h"
#include <fstream>
#include <stdexcept>

namespace IO {

    ConfigData loadConfiguration(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Nao foi possivel abrir o arquivo de entrada.");
        }

        ConfigData data;
        std::string trash;

        file >> data.transportCapacity >> data.transportLatency >> data.transportInterval >> data.removalCost;

        file >> data.numWarehouses;

        data.graph = new Graph(data.numWarehouses);

        data.warehouses = new Warehouse*[data.numWarehouses];
        
        for (int i = 0; i < data.numWarehouses; ++i) {
            data.warehouses[i] = new Warehouse(i, data.numWarehouses);
        }

        for (int i = 0; i < data.numWarehouses; ++i) {
            for (int j = 0; j < data.numWarehouses; ++j) {
                int connection;
                file >> connection;
                if (connection == 1 && i < j) {
                    data.graph->addEdge(i, j, data.transportCapacity);
                }
            }
        }
        
        file >> data.numPackages;
        data.packages = new Package*[data.numPackages];
        for (int i = 0; i < data.numPackages; ++i) {
            int postTime, id, origin, destination;
            file >> postTime >> trash >> id >> trash >> origin >> trash >> destination;
            data.packages[i] = new Package(id, origin, destination, postTime);
        }

        file.close();
        return data;
    }

} 
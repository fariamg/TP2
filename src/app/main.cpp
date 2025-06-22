#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../include/core/Scheduler.h"
#include "../include/dataStructures/Graph.h"
#include "../include/domains/Package.h"
#include "../include/domains/Warehouse.h"

//@ Estrutura que guarda os dados de configuração lidos do arquivo de entrada
struct ConfigData {
    int transportCapacity;
    int transportLatency;
    int transportInterval;
    int removalCost;
    int numWarehouses;
    int numPackages;
    Graph* graph;
    Warehouse** warehouses;
    Package** packages;
};

//@ Funções auxiliares de log
void printFormattedTime(int time) {
    std::cout << std::setw(7) << std::setfill('0') << time;
}

void logPackageStored(int time, int pkgId, int whId, int sectionId) {
    printFormattedTime(time);
    std::cout << " pacote " << std::setw(3) << pkgId << " armazenado em " << std::setw(3) << whId << " na secao " << std::setw(3) << sectionId << std::endl;
}

void logPackageInTransit(int time, int pkgId, int originId, int destId) {
    printFormattedTime(time);
    std::cout << " pacote " << std::setw(3) << pkgId << " em transito de " << std::setw(3) << originId << " para " << std::setw(3) << destId << std::endl;
}

void logPackageDelivered(int time, int pkgId, int whId) {
    printFormattedTime(time);
    std::cout << " pacote " << std::setw(3) << pkgId << " entregue em " << std::setw(3) << whId << std::endl;
}

void logPackageRemoved(int time, int pkgId, int whId, int sectionId) {
    printFormattedTime(time);
    std::cout << " pacote " << std::setw(3) << pkgId << " removido de " << std::setw(3) << whId << " na secao " << std::setw(3) << sectionId << std::endl;
}

void logPackageRestored(int time, int pkgId, int whId, int sectionId) {
    printFormattedTime(time);
    std::cout << " pacote " << std::setw(3) << pkgId << " rearmazenado em " << std::setw(3) << whId << " na secao " << std::setw(3) << sectionId << std::endl;
}

//@ Funções auxiliares para leitura do arquivo de entrada
void readGeneralConfig(std::ifstream& file, ConfigData& data) {
    file >> data.transportCapacity >> data.transportLatency >> data.transportInterval >> data.removalCost;
    if (file.fail()) {
        throw std::runtime_error("Erro ao ler configuracoes gerais do arquivo.");
    }
}

void readWarehouseAndGraphData(std::ifstream& file, ConfigData& data) {
    file >> data.numWarehouses;
    if (file.fail() || data.numWarehouses <= 0) {
        throw std::runtime_error("Erro ao ler numero de armazens ou numero invalido.");
    }

    data.graph = new Graph(data.numWarehouses);
    data.warehouses = new Warehouse*[data.numWarehouses];
    for (int i = 0; i < data.numWarehouses; ++i) {
        data.warehouses[i] = new Warehouse(i, data.numWarehouses);
    }

    for (int i = 0; i < data.numWarehouses; ++i) {
        for (int j = 0; j < data.numWarehouses; ++j) {
            int connection;
            file >> connection;
            if (file.fail()) {
                throw std::runtime_error("Erro ao ler matriz de adjacencia dos armazens.");
            }
            if (connection == 1 && i < j) {
                data.graph->addEdge(i, j, data.transportCapacity);
            }
        }
    }
}

void readPackageData(std::ifstream& file, ConfigData& data) {
    file >> data.numPackages;
    if (file.fail() || data.numPackages < 0) {
        throw std::runtime_error("Erro ao ler numero de pacotes ou numero invalido.");
    }
    if (data.numPackages == 0) {
        data.packages = nullptr;
        return;
    }

    data.packages = new Package*[data.numPackages];
    std::string trash;

    for (int i = 0; i < data.numPackages; ++i) {
        int postTime, origin, destination;
        file >> postTime >> trash >> trash >> trash >> origin >> trash >> destination;
        if (file.fail()) {
            throw std::runtime_error("Erro ao ler dados do pacote " + std::to_string(i));
        }
        if (origin < 0 || origin >= data.numWarehouses || destination < 0 || destination >= data.numWarehouses) {
            throw std::runtime_error("Origem ou destino invalido para o pacote " + std::to_string(i));
        }

        data.packages[i] = new Package(i, origin, destination, postTime);
    }
}

ConfigData loadInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de entrada: " + filename);
    }

    ConfigData data;

    try {
        readGeneralConfig(file, data);
        readWarehouseAndGraphData(file, data);
        readPackageData(file, data);
    } catch (const std::exception& e) {
        file.close();
        throw;
    }

    file.close();
    return data;
}

void cleanup(ConfigData& data) {
    if (data.graph) {
        delete data.graph;
    }
    if (data.warehouses) {
        for (int i = 0; i < data.numWarehouses; ++i) {
            delete data.warehouses[i];
        }
        delete[] data.warehouses;
    }
    if (data.packages) {
        for (int i = 0; i < data.numPackages; ++i) {
            delete data.packages[i];
        }
        delete[] data.packages;
    }
    data = ConfigData();
}

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
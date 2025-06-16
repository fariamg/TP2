#include <fstream>
#include <stdexcept>
#include <vector>

#include "../include/core/RouteCalculator.h"
#include "../include/dataStructures/Graph.h"
#include "../include/domains/Package.h"
#include "../include/domains/Warehouse.h"
#include "../include/io/InputLoader.h"

namespace IO {

static void readGeneralConfig(std::ifstream &file, ConfigData &data) {
  file >> data.transportCapacity >> data.transportLatency >>
      data.transportInterval >> data.removalCost;
  if (file.fail()) {
    throw std::runtime_error("Erro ao ler configuracoes gerais do arquivo.");
  }
}

static void readWarehouseAndGraphData(std::ifstream &file, ConfigData &data) {
  file >> data.numWarehouses;
  if (file.fail() || data.numWarehouses <= 0) {
    throw std::runtime_error(
        "Erro ao ler numero de armazens ou numero invalido.");
  }

  data.graph = new Graph(data.numWarehouses);
  data.warehouses = new Warehouse *[data.numWarehouses];
  for (int i = 0; i < data.numWarehouses; ++i) {
    data.warehouses[i] = new Warehouse(i, data.numWarehouses);
  }

  for (int i = 0; i < data.numWarehouses; ++i) {
    for (int j = 0; j < data.numWarehouses; ++j) {
      int connection;
      file >> connection;
      if (file.fail()) {
        throw std::runtime_error(
            "Erro ao ler matriz de adjacencia dos armazens.");
      }
      if (connection == 1 && i < j) {
        data.graph->addEdge(i, j, data.transportCapacity);
      }
    }
  }
}

static void readPackageData(std::ifstream &file, ConfigData &data) {
  file >> data.numPackages;
  if (file.fail() || data.numPackages < 0) {
    throw std::runtime_error(
        "Erro ao ler numero de pacotes ou numero invalido.");
  }
  if (data.numPackages == 0) {
    data.packages = nullptr;
    return;
  }

  data.packages = new Package *[data.numPackages];
  std::string trash;

  for (int i = 0; i < data.numPackages; ++i) {
    int postTime, id, origin, destination;
    file >> postTime >> trash >> id >> trash >> origin >> trash >> destination;
    if (file.fail()) {
      throw std::runtime_error("Erro ao ler dados do pacote " +
                               std::to_string(i));
    }
    if (origin < 0 || origin >= data.numWarehouses || destination < 0 ||
        destination >= data.numWarehouses) {
      throw std::runtime_error("Origem ou destino invalido para o pacote " +
                               std::to_string(id));
    }

    data.packages[i] = new Package(id, origin, destination, postTime);

    data.packages[i]->setRoute(
        Routing::calculateOptimalRoute(origin, destination, *data.graph));
  }
}

ConfigData loadInput(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Nao foi possivel abrir o arquivo de entrada: " +
                             filename);
  }

  ConfigData data;

  try {
    readGeneralConfig(file, data);
    readWarehouseAndGraphData(file, data);
    readPackageData(file, data);
  } catch (const std::exception &e) {
    file.close();
    throw;
  }

  file.close();
  return data;
}

} // namespace IO
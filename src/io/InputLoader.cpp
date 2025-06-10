#include "io/InputLoader.h"
#include "domains/Warehouse.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

InputLoader::ConfigData InputLoader::loadInput(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Nao foi possivel abrir o arquivo de entrada.");
  }

  ConfigData data;
  std::string line;

  file >> data.transportCapacity >> data.transportLatency >>
      data.transportInterval >> data.removalCost;

  file >> data.numWarehouses;
  data.graph = new Graph(data.numWarehouses);
  for (int i = 0; i < data.numWarehouses; ++i) {
    for (int j = 0; j < data.numWarehouses; ++j) {
      int connection;
      file >> connection;
      if (connection == 1) {
        if (i < j) {
          data.graph->addEdge(i, j, data.transportCapacity);
        }
      }
    }
  }

  file >> data.numPackages;
  data.packages = new Package *[data.numPackages];
  for (int i = 0; i < data.numPackages; ++i) {
    int postTime, id, origin, destination;
    std::string pac, org, dst; 
    file >> postTime >> pac >> id >> org >> origin >> dst >> destination;
    data.packages[i] = new Package(id, origin, destination, postTime);
  }

  file.close();
  return data;
}
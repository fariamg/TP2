#pragma once

#include "Graph.h"
#include "Package.h"

class InputLoader {
public:
  struct ConfigData {
    int transportCapacity;
    int transportLatency;
    int transportInterval;
    int removalCost;
    int numWarehouses;
    int numPackages;
    Graph *graph;
    Package **packages;
  };

  ConfigData loadInput(const std::string &filePath);
};
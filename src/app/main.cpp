#include <iostream>
#include <stdexcept>
#include <string>

// #include "../include/core/Simulation.h" //TODO: IMPLEMENTAR O SIMULATION
#include "../include/io/InputLoader.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Erro: Arquivo de entrada nao especificado." << std::endl;
    std::cerr << "Uso: " << argv[0] << " <caminho_para_o_arquivo_de_entrada>"
              << std::endl;
    return 1;
  }

  std::string inputFilename = argv[1];

  try {

    IO::ConfigData data = IO::loadInput(inputFilename);

    // TESTE DE INPUTS
    std::cout << "Capacidade de transporte: " << data.transportCapacity
              << std::endl;
    std::cout << "Latencia de transporte: " << data.transportLatency
              << std::endl;
    std::cout << "Intervalo de transporte: " << data.transportInterval
              << std::endl;
    std::cout << "Custo de remocao: " << data.removalCost << std::endl;
    std::cout << "Numero de armazens: " << data.numWarehouses << std::endl;
    std::cout << "Numero de pacotes: " << data.numPackages << std::endl;
    for (int i = 0; i < data.numWarehouses; ++i) {
      std::cout << "Armazem " << i << ": ID = " << data.warehouses[i]->getId()
                << std::endl;
    }

    for (int i = 0; i < data.numPackages; ++i) {
      std::cout << "Pacote " << i << ": ID = " << data.packages[i]->getId()
                << ", Origem = " << data.packages[i]->getInitialOrigin()
                << ", Destino = " << data.packages[i]->getFinalDestination()
                << std::endl;
    }

    // MOSTRA AS CONEXÕES ENTRE OS ARMAZÉNS
    std::cout << "Conexoes entre armazens:" << std::endl;
    for (int i = 0; i < data.numWarehouses; ++i) {
      for (int j = 0; j < data.numWarehouses; ++j) {
        if (data.graph->hasEdge(i, j)) {
          std::cout << "Armazem " << i << " -> Armazem " << j
                    << " com capacidade de transporte: " << std::endl;
        }
      }
    }

    // Simulation sim(data);

    // sim.setupInitialEvents();

    // sim.run();

  } catch (const std::exception &e) {
    std::cerr << "Ocorreu um erro durante a execucao: " << e.what()
              << std::endl;
    return 1;
  }

  return 0;
}
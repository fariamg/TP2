#include "../../include/analysis/SimulationMetrics.h"
#include "../../include/analysis/ConfigGenerator.h"
#include "../../include/core/ConfigData.h"
#include "../../include/core/Scheduler.h"
#include "../../include/domains/Package.h"
#include "../../include/domains/Warehouse.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

// Global metrics instance to collect data during simulation
SimulationMetrics* g_metrics = nullptr;

SimulationMetrics::SimulationMetrics() : rearrangementCount(0), transportEventCount(0), maxOccupancy(0) {
    deliveryTimes.clear();
    waitTimes.clear();
    totalProcessingTime = 0;
    actualSimulationTime = 0;
}

void SimulationMetrics::startTimer() {
    startTime = std::chrono::high_resolution_clock::now();
}

void SimulationMetrics::stopTimer() {
    endTime = std::chrono::high_resolution_clock::now();
}

void SimulationMetrics::recordRearrangement() {
    rearrangementCount++;
}

void SimulationMetrics::recordTransportEvent() {
    transportEventCount++;
}

void SimulationMetrics::recordDeliveryTime(double time) {
    deliveryTimes.push_back(time);
    if (time > actualSimulationTime) {
        actualSimulationTime = time;
    }
}

void SimulationMetrics::recordWaitTime(double time) {
    waitTimes.push_back(time);
}

void SimulationMetrics::updateMaxOccupancy(int occupancy) {
    if (occupancy > maxOccupancy) {
        maxOccupancy = occupancy;
    }
}

void SimulationMetrics::recordProcessingOperation() {
    totalProcessingTime++;
}

SimulationMetrics::Metrics SimulationMetrics::calculateMetrics(const ConfigData& config) {
    Metrics metrics;

    // Calcula tempo de processamento em millisegundos
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    metrics.executionTimeMs = duration.count() / 1000.0;

    metrics.totalPackages = config.numPackages;

    // Count actual delivered packages by checking package states
    int actualDelivered = 0;
    double totalDeliveryTime = 0;

    for (int i = 0; i < config.numPackages; ++i) {
        Package* pkg = config.packages[i];
        if (pkg != nullptr && pkg->getState() == PackageState::DELIVERED) {
            actualDelivered++;
            // Calcula tempo de entrega real (tempo final - tempo de postagem)
            if (!deliveryTimes.empty() && static_cast<size_t>(i) < deliveryTimes.size()) {
                totalDeliveryTime += (deliveryTimes[i] - pkg->getPostTime());
            }
        }
    }

    metrics.deliveredPackages = actualDelivered;
    metrics.totalRearrangements = rearrangementCount;
    metrics.totalTransportEvents = transportEventCount;
    metrics.maxWarehouseOccupancy = maxOccupancy;

    // Calcula tempo médio de entrega baseado nos pacotes realmente entregues
    if (actualDelivered > 0) {
        metrics.averageDeliveryTime = totalDeliveryTime / actualDelivered;
    } else {
        metrics.averageDeliveryTime = 0;
    }

    // Calcula tempo médio de espera
    if (!waitTimes.empty()) {
        double sum = 0;
        for (double time : waitTimes) {
            sum += time;
        }
        metrics.averageWaitTime = sum / waitTimes.size();
    } else {
        metrics.averageWaitTime = 0;
    }

    // Calcula throughput baseado no tempo de simulação real, não processamento
    if (actualSimulationTime > 0) {
        metrics.throughput = actualDelivered / actualSimulationTime; // pacotes por unidade de tempo da simulação
    } else if (metrics.executionTimeMs > 0) {
        // Fallback: throughput de processamento
        metrics.throughput = (actualDelivered * 1000.0) / metrics.executionTimeMs;
    } else {
        metrics.throughput = 0;
    }

    metrics.totalEvents = rearrangementCount + transportEventCount;
    metrics.packagesInTransit = config.numPackages - metrics.deliveredPackages;

    return metrics;
}

SimulationMetrics::Metrics SimulationMetrics::runSimulationWithMetrics(const std::string& configFile) {
    ConfigData data = {};
    SimulationMetrics metrics;
    g_metrics = &metrics; // Set global pointer for logging functions to use

    try {
        // Medição inclui TODO o processamento
        metrics.startTimer();

        std::cout << "Loading config: " << configFile << std::endl;
        data = loadInput(configFile);

        // Verifica se dados foram carregados corretamente
        if (data.packages == nullptr && data.numPackages > 0) {
            std::cerr << "Erro: Pacotes não foram carregados corretamente" << std::endl;
            cleanup(data);
            g_metrics = nullptr;
            return SimulationMetrics::Metrics{};
        }

        if (data.warehouses == nullptr || data.graph == nullptr) {
            std::cerr << "Erro: Armazéns ou grafo não foram carregados" << std::endl;
            cleanup(data);
            g_metrics = nullptr;
            return SimulationMetrics::Metrics{};
        }

        std::cout << "Loaded " << data.numPackages << " packages, " << data.numWarehouses << " warehouses" << std::endl;

        // Calcula rotas otimais (parte da medição)
        for (int i = 0; i < data.numPackages; ++i) {
            Package* pkg = data.packages[i];
            if (pkg != nullptr) {
                LinkedList route = Routing::calculateOptimalRoute(pkg->getInitialOrigin(), pkg->getFinalDestination(), *data.graph);
                pkg->setRoute(std::move(route));
                metrics.recordProcessingOperation(); // Conta operações de processamento
            }
        }

        // Executa simulação (parte principal da medição)
        Scheduler scheduler(data);
        std::cout << "Starting simulation..." << std::endl;
        scheduler.runSimulation(data.warehouses, data.graph, data.numWarehouses);
        std::cout << "Simulation completed" << std::endl;

        // Para timer após toda a operação
        metrics.stopTimer();

        // Coleta métricas adicionais do estado final
        int totalOccupancy = 0;
        for (int i = 0; i < data.numWarehouses; i++) {
            if (data.warehouses[i] != nullptr) {
                for (int j = 0; j < data.numWarehouses; j++) {
                    totalOccupancy += data.warehouses[i]->getSectionSize(j);
                }
            }
        }
        metrics.updateMaxOccupancy(totalOccupancy);

        // Count delivered packages by checking their state
        int deliveredCount = 0;
        for (int i = 0; i < data.numPackages; ++i) {
            Package* pkg = data.packages[i];
            if (pkg != nullptr && pkg->getState() == PackageState::DELIVERED) {
                deliveredCount++;
            }
        }

        std::cout << "Delivered " << deliveredCount << " out of " << data.numPackages << " packages" << std::endl;
        std::cout << "Rearrangements: " << metrics.rearrangementCount << std::endl;
        std::cout << "Transport events: " << metrics.transportEventCount << std::endl;
        std::cout << "Processing time: " << metrics.totalProcessingTime << " operations" << std::endl;
        std::cout << "Simulation time: " << metrics.actualSimulationTime << " time units" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erro durante simulação: " << e.what() << std::endl;
        cleanup(data);
        g_metrics = nullptr;
        return SimulationMetrics::Metrics{};
    }

    auto result = metrics.calculateMetrics(data);
    cleanup(data);
    g_metrics = nullptr;
    return result;
}

SimulationMetrics::ExperimentResult SimulationMetrics::runWarehouseScalingExperiment() {
    ExperimentResult result;
    result.experimentName = "Escalabilidade por Número de Armazéns";

    // Configura experimento: 5 a 25 armazéns, PACOTES FIXOS
    auto configs = ConfigGenerator::generateWarehouseVariations(5, 25, 5, // min, max, step
                                                                200,      // PACOTES FIXOS
                                                                3, 20, 30 // capacidade, latência, intervalo base
    );

    // Remove arquivos antigos
    std::filesystem::remove_all("results/warehouse_scaling");

    // Cria diretório de resultados se não existir
    std::error_code ec;
    std::filesystem::create_directories("results/warehouse_scaling", ec);
    if (ec) {
        std::cerr << "Erro ao criar diretório: " << ec.message() << std::endl;
        return result;
    }

    std::cout << "Running warehouse scaling experiment with " << configs.size() << " configurations..." << std::endl;

    for (const auto& config : configs) {
        std::string filename = "results/warehouse_scaling/" + config.description + ".txt";

        std::cout << "\n=== Testing configuration: " << config.description << " ===" << std::endl;
        std::cout << "Warehouses: " << config.numWarehouses << ", Packages: " << config.numPackages << std::endl;

        if (ConfigGenerator::saveConfigToFile(config, filename)) {
            auto metrics = runSimulationWithMetrics(filename);

            result.inputSizes.push_back(config.numWarehouses);
            result.executionTimes.push_back(metrics.executionTimeMs);
            result.allMetrics.push_back(metrics);

            std::cout << "RESULT - Armazéns: " << config.numWarehouses << ", Tempo: " << metrics.executionTimeMs << "ms" << ", Pacotes entregues: " << metrics.deliveredPackages << "/"
                      << metrics.totalPackages << ", Rearmazenamentos: " << metrics.totalRearrangements << ", Taxa entrega: " << (100.0 * metrics.deliveredPackages / metrics.totalPackages) << "%"
                      << ", Throughput: " << metrics.throughput << " pct/unidade_tempo" << std::endl;
        } else {
            std::cerr << "Failed to save config file: " << filename << std::endl;
        }
    }

    return result;
}

SimulationMetrics::ExperimentResult SimulationMetrics::runPackageScalingExperiment() {
    ExperimentResult result;
    result.experimentName = "Escalabilidade por Número de Pacotes";

    // Configura experimento: 100 a 500 pacotes, ARMAZÉNS FIXOS
    auto configs = ConfigGenerator::generatePackageVariations(100, 500, 100, // min, max, step
                                                              10,            // ARMAZÉNS FIXOS
                                                              3, 20, 30      // capacidade, latência, intervalo base
    );

    // Remove arquivos antigos
    std::filesystem::remove_all("results/package_scaling");

    std::error_code ec;
    std::filesystem::create_directories("results/package_scaling", ec);
    if (ec) {
        std::cerr << "Erro ao criar diretório: " << ec.message() << std::endl;
        return result;
    }

    std::cout << "Running package scaling experiment with " << configs.size() << " configurations..." << std::endl;

    for (const auto& config : configs) {
        std::string filename = "results/package_scaling/" + config.description + ".txt";

        std::cout << "\n=== Testing configuration: " << config.description << " ===" << std::endl;
        std::cout << "Warehouses: " << config.numWarehouses << ", Packages: " << config.numPackages << std::endl;

        if (ConfigGenerator::saveConfigToFile(config, filename)) {
            auto metrics = runSimulationWithMetrics(filename);

            result.inputSizes.push_back(config.numPackages);
            result.executionTimes.push_back(metrics.executionTimeMs);
            result.allMetrics.push_back(metrics);

            std::cout << "RESULT - Pacotes: " << config.numPackages << ", Tempo: " << metrics.executionTimeMs << "ms" << ", Pacotes entregues: " << metrics.deliveredPackages << "/"
                      << metrics.totalPackages << ", Rearmazenamentos: " << metrics.totalRearrangements << ", Taxa entrega: " << (100.0 * metrics.deliveredPackages / metrics.totalPackages) << "%"
                      << ", Throughput: " << metrics.throughput << " pct/unidade_tempo" << std::endl;
        } else {
            std::cerr << "Failed to save config file: " << filename << std::endl;
        }
    }

    return result;
}

SimulationMetrics::ExperimentResult SimulationMetrics::runTransportContentionExperiment() {
    ExperimentResult result;
    result.experimentName = "Análise de Contenção por Transporte";

    auto configs = ConfigGenerator::generateTransportContentionVariations(10, 300);

    // Remove arquivos antigos
    std::filesystem::remove_all("results/transport_contention");

    std::error_code ec;
    std::filesystem::create_directories("results/transport_contention", ec);
    if (ec) {
        std::cerr << "Erro ao criar diretório: " << ec.message() << std::endl;
        return result;
    }

    std::cout << "Running transport contention experiment with " << configs.size() << " configurations..." << std::endl;

    for (const auto& config : configs) {
        std::string filename = "results/transport_contention/" + config.description + ".txt";

        std::cout << "\n=== Testing configuration: " << config.description << " ===" << std::endl;
        std::cout << "Capacity: " << config.transportCapacity << ", Latency: " << config.transportLatency << ", Interval: " << config.transportInterval << std::endl;

        if (ConfigGenerator::saveConfigToFile(config, filename)) {
            auto metrics = runSimulationWithMetrics(filename);

            // Para contenção, usamos configuração como input size
            result.inputSizes.push_back(config.transportCapacity * 100 + config.transportInterval);
            result.executionTimes.push_back(metrics.executionTimeMs);
            result.allMetrics.push_back(metrics);

            std::cout << "RESULT - Config: " << config.description << ", Rearmazenamentos: " << metrics.totalRearrangements << ", Pacotes entregues: " << metrics.deliveredPackages << "/"
                      << metrics.totalPackages << ", Tempo: " << metrics.executionTimeMs << "ms" << ", Taxa entrega: " << (100.0 * metrics.deliveredPackages / metrics.totalPackages) << "%"
                      << ", Contenção: " << (100.0 * metrics.totalRearrangements / metrics.totalPackages) << "%" << std::endl;
        } else {
            std::cerr << "Failed to save config file: " << filename << std::endl;
        }
    }

    return result;
}

void SimulationMetrics::saveResultsToCSV(const ExperimentResult& results, const std::string& filename) {
    // Remove arquivo antigo se existir
    std::remove(filename.c_str());

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao criar arquivo CSV: " << filename << std::endl;
        return;
    }

    // Cabeçalho CSV (apenas uma vez)
    file << "Input_Size,Execution_Time_MS,Total_Packages,Delivered_Packages,Rearrangements,"
         << "Transport_Events,Max_Occupancy,Avg_Delivery_Time,Avg_Wait_Time,Throughput,Delivery_Rate,Contention_Rate\n";

    for (size_t i = 0; i < results.inputSizes.size(); i++) {
        if (i < results.allMetrics.size()) {
            const auto& metrics = results.allMetrics[i];
            double deliveryRate = metrics.totalPackages > 0 ? (100.0 * metrics.deliveredPackages / metrics.totalPackages) : 0.0;
            double contentionRate = metrics.totalPackages > 0 ? (100.0 * metrics.totalRearrangements / metrics.totalPackages) : 0.0;

            file << results.inputSizes[i] << "," << metrics.executionTimeMs << "," << metrics.totalPackages << "," << metrics.deliveredPackages << "," << metrics.totalRearrangements << ","
                 << metrics.totalTransportEvents << "," << metrics.maxWarehouseOccupancy << "," << metrics.averageDeliveryTime << "," << metrics.averageWaitTime << "," << metrics.throughput << ","
                 << deliveryRate << "," << contentionRate << "\n";
        }
    }

    file.close();
    std::cout << "Results saved to: " << filename << std::endl;
}

void SimulationMetrics::generateGraph(const ExperimentResult& results, const std::string& title, const std::string& outputFile) {
    if (results.inputSizes.empty() || results.executionTimes.empty()) {
        std::cerr << "No data to generate graph for: " << title << std::endl;
        return;
    }

    // Cria arquivo de dados temporário
    std::string dataFile = outputFile + ".dat";
    std::ofstream data(dataFile);
    if (!data.is_open()) {
        std::cerr << "Erro ao criar arquivo de dados: " << dataFile << std::endl;
        return;
    }

    for (size_t i = 0; i < results.inputSizes.size() && i < results.executionTimes.size(); i++) {
        data << results.inputSizes[i] << " " << results.executionTimes[i] << "\n";
    }
    data.close();

    // Cria script gnuplot
    std::string scriptFile = outputFile + ".gp";
    std::ofstream script(scriptFile);
    if (!script.is_open()) {
        std::cerr << "Erro ao criar script gnuplot: " << scriptFile << std::endl;
        return;
    }

    script << "set terminal pngcairo enhanced font 'Arial,12' size 800,600\n";
    script << "set output '" << outputFile << ".png'\n";
    script << "set title '" << title << "'\n";
    script << "set xlabel 'Tamanho da Entrada'\n";
    script << "set ylabel 'Tempo de Execução (ms)'\n";
    script << "set grid\n";
    script << "set key top left\n";
    script << "plot '" << dataFile << "' using 1:2 with linespoints title 'Tempo Experimental' linecolor rgb 'blue',\\\n";

    // Adiciona curva teórica baseada em complexidade
    if (results.experimentName.find("Armazéns") != std::string::npos) {
        script << "     x*x*log(x)/100 title 'Curva O(n²log(n))' linecolor rgb 'red'\n";
    } else if (results.experimentName.find("Pacotes") != std::string::npos) {
        script << "     x*log(x)/10 title 'Curva O(n log(n))' linecolor rgb 'red'\n";
    } else {
        script << "     x*x/1000 title 'Curva Quadrática' linecolor rgb 'red'\n";
    }

    script.close();

    // Executa gnuplot
    std::string command = "gnuplot " + scriptFile;
    int result_code = system(command.c_str());

    if (result_code == 0) {
        std::cout << "Gráfico gerado: " << outputFile << ".png" << std::endl;
    } else {
        std::cout << "Erro ao gerar gráfico. Verifique se gnuplot está instalado." << std::endl;
    }

    // Remove arquivos temporários
    std::remove(dataFile.c_str());
    std::remove(scriptFile.c_str());
}

// Global functions to be called from logging functions to collect metrics
extern "C" void recordMetricsRearrangement() {
    if (g_metrics) {
        g_metrics->recordRearrangement();
    }
}

extern "C" void recordMetricsTransportEvent() {
    if (g_metrics) {
        g_metrics->recordTransportEvent();
    }
}

extern "C" void recordMetricsDelivery(double time) {
    if (g_metrics) {
        g_metrics->recordDeliveryTime(time);
    }
}
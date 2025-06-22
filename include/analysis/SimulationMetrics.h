#pragma once

#include <chrono>
#include <string>
#include <vector>

//@ Forward declarations
struct ConfigData;
class Warehouse;
class Graph;

//@ Classe responsável por coletar métricas durante a simulação
class SimulationMetrics {
  public:
    //@ Estrutura que armazena as métricas coletadas
    struct Metrics {
        double executionTimeMs;
        int totalPackages;
        int deliveredPackages;
        int totalRearrangements;
        int totalTransportEvents;
        int maxWarehouseOccupancy;
        double averageDeliveryTime;
        int totalEvents;

        // Métricas específicas de contenção
        double averageWaitTime;
        int packagesInTransit;
        double throughput; // pacotes/unidade_tempo_simulacao
    };

    //@ Estrutura para armazenar resultados de múltiplos testes
    struct ExperimentResult {
        std::vector<int> inputSizes;
        std::vector<double> executionTimes;
        std::vector<Metrics> allMetrics;
        std::string experimentName;
    };

  private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;

    // Contadores de métricas
    int rearrangementCount;
    int transportEventCount;
    int maxOccupancy;
    std::vector<double> deliveryTimes;
    std::vector<double> waitTimes;

    // Novas métricas para medição mais precisa
    int totalProcessingTime;
    double actualSimulationTime;

  public:
    //@ Construtor
    SimulationMetrics();

    //@ Inicia a medição de tempo
    void startTimer();

    //@ Para a medição de tempo
    void stopTimer();

    //@ Incrementa contador de rearmazenamentos
    void recordRearrangement();

    //@ Incrementa contador de eventos de transporte
    void recordTransportEvent();

    //@ Registra tempo de entrega de um pacote
    void recordDeliveryTime(double time);

    //@ Registra tempo de espera
    void recordWaitTime(double time);

    //@ Atualiza ocupação máxima dos armazéns
    void updateMaxOccupancy(int occupancy);

    //@ Registra operação de processamento
    void recordProcessingOperation();

    //@ Calcula e retorna as métricas finais
    Metrics calculateMetrics(const ConfigData& config);

    //@ Executa uma simulação e coleta métricas
    static Metrics runSimulationWithMetrics(const std::string& configFile);

    //@ Executa experimento variando número de armazéns
    static ExperimentResult runWarehouseScalingExperiment();

    //@ Executa experimento variando número de pacotes
    static ExperimentResult runPackageScalingExperiment();

    //@ Executa experimento variando contenção de transporte
    static ExperimentResult runTransportContentionExperiment();

    //@ Salva resultados em arquivo CSV
    static void saveResultsToCSV(const ExperimentResult& results, const std::string& filename);

    //@ Gera gráfico dos resultados (usando gnuplot)
    static void generateGraph(const ExperimentResult& results, const std::string& title, const std::string& outputFile);
};
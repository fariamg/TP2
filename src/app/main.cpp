#include <iostream>
#include <string>
#include <stdexcept>

#include "../include/core/Simulation.h" //TODO: IMPLEMENTAR O SIMULATION
#include "io/InputLoader.h"

int main(int argc, char* argv[]) {
    // 1. Verifica se o nome do arquivo de entrada foi fornecido na linha de comando.
    if (argc < 2) {
        // Imprime a mensagem de erro na saída de erro padrão (stderr)
        std::cerr << "Erro: Arquivo de entrada nao especificado." << std::endl;
        std::cerr << "Uso: " << argv[0] << " <caminho_para_o_arquivo_de_entrada>" << std::endl;
        return 1; // Retorna um código de erro
    }

    // O nome do arquivo é o primeiro argumento após o nome do programa
    std::string inputFilename = argv[1];

    // 2. O bloco try...catch captura quaisquer erros que possam ocorrer durante
    //    o carregamento do arquivo ou a execução da simulação.
    try {
        // ETAPA 1: CARREGAR
        // Usa a função livre do namespace IO para carregar toda a configuração.
        // O objeto 'data' conterá todos os ponteiros e parâmetros iniciais.
        IO::ConfigData data = IO::loadInput(inputFilename);
        
        // ETAPA 2: CRIAR
        // Cria a instância principal da Simulação, passando os dados carregados.
        // A partir deste ponto, o objeto 'sim' é o "dono" da memória alocada.
        Simulation sim(data);
        
        // ETAPA 3: CONFIGURAR
        // Calcula as rotas e agenda os primeiros eventos na fila de prioridade.
        sim.setupInitialEvents();
        
        // ETAPA 4: EXECUTAR
        // Inicia o laço principal da simulação.
        sim.run();

    } catch (const std::exception& e) {
        // Se qualquer parte do código lançar uma exceção padrão (ex: arquivo não encontrado),
        // ela será capturada aqui.
        std::cerr << "Ocorreu um erro durante a execucao: " << e.what() << std::endl;
        return 1; // Retorna um código de erro
    }

    // Se o programa chegar aqui, a execução foi bem-sucedida.
    return 0;
}
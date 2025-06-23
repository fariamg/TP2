#include "../../include/utils/Logger.h"
#include <iomanip>
#include <iostream>

// Funções adicionais para logging dos pontos extras

void logDynamicRouteCalculation(int time, int pkgId, int oldRoute, int newRoute) {
    printFormattedTime(time);
    std::cout << " pacote " << std::setw(3) << pkgId << " rota recalculada de " << std::setw(3) << oldRoute << " para " << std::setw(3) << newRoute << std::endl;
}

void logCapacityOverflow(int time, int pkgId, int whId, int sectionId) {
    printFormattedTime(time);
    std::cout << " OVERFLOW: pacote " << std::setw(3) << pkgId << " no armazém " << std::setw(3) << whId << " seção " << std::setw(3) << sectionId << std::endl;
}

void logWeightExceeded(int time, int pkgId, int weight, int whId) {
    printFormattedTime(time);
    std::cout << " PESO EXCEDIDO: pacote " << std::setw(3) << pkgId << " peso " << std::setw(3) << weight << "kg no armazém " << std::setw(3) << whId << std::endl;
}

void logAlternativeRoute(int time, int pkgId, int alternativeWh) {
    printFormattedTime(time);
    std::cout << " ROTA ALTERNATIVA: pacote " << std::setw(3) << pkgId << " redirecionado para armazém " << std::setw(3) << alternativeWh << std::endl;
}

void logSecondaryStorage(int time, int pkgId, int primaryWh, int secondaryWh) {
    printFormattedTime(time);
    std::cout << " ARMAZENAMENTO SECUNDÁRIO: pacote " << std::setw(3) << pkgId << " transferido de " << std::setw(3) << primaryWh << " para " << std::setw(3) << secondaryWh << std::endl;
}

void logVariableTransportTime(int time, int origin, int destination, int dynamicLatency, int baseLatency) {
    printFormattedTime(time);
    std::cout << " TEMPO VARIÁVEL: rota " << std::setw(3) << origin << "->" << std::setw(3) << destination << " latência " << dynamicLatency << " (base: " << baseLatency << ")" << std::endl;
}

void logPackageWeight(int time, int pkgId, int weight, const char* weightClass) {
    printFormattedTime(time);
    std::cout << " PESO: pacote " << std::setw(3) << pkgId << " peso " << std::setw(3) << weight << "kg classe " << weightClass << std::endl;
}

void logWaitingQueue(int time, int pkgId, int whId, int queueSize) {
    printFormattedTime(time);
    std::cout << " FILA ESPERA: pacote " << std::setw(3) << pkgId << " em armazém " << std::setw(3) << whId << " posição " << std::setw(3) << queueSize << std::endl;
}
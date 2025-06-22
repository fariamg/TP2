#include "../../include/utils/Logger.h"
#include <iomanip>
#include <iostream>

// External functions to record metrics (when available)
extern "C" void recordMetricsRearrangement();
extern "C" void recordMetricsTransportEvent();
extern "C" void recordMetricsDelivery(double time);

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

    // Record transport event for metrics
    recordMetricsTransportEvent();
}

void logPackageDelivered(int time, int pkgId, int whId) {
    printFormattedTime(time);
    std::cout << " pacote " << std::setw(3) << pkgId << " entregue em " << std::setw(3) << whId << std::endl;

    // Record delivery for metrics
    recordMetricsDelivery(static_cast<double>(time));
}

void logPackageRemoved(int time, int pkgId, int whId, int sectionId) {
    printFormattedTime(time);
    std::cout << " pacote " << std::setw(3) << pkgId << " removido de " << std::setw(3) << whId << " na secao " << std::setw(3) << sectionId << std::endl;
}

void logPackageRestored(int time, int pkgId, int whId, int sectionId) {
    printFormattedTime(time);
    std::cout << " pacote " << std::setw(3) << pkgId << " rearmazenado em " << std::setw(3) << whId << " na secao " << std::setw(3) << sectionId << std::endl;

    // Record rearrangement for metrics
    recordMetricsRearrangement();
}
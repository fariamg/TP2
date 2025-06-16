#include "ui/OutputLogger.h"
#include <iomanip>
#include <iostream>

void OutputLogger::printFormattedTime(int time) {
  std::cout << std::setw(7) << std::setfill('0') << time;
}

void OutputLogger::logPackageStored(int time, int pkgId, int whId,
                                    int sectionId) {
  printFormattedTime(time);
  std::cout << " pacote " << std::setw(3) << pkgId << " armazenado em "
            << std::setw(3) << whId << " na secao " << std::setw(3) << sectionId
            << std::endl;
}

void OutputLogger::logPackageInTransit(int time, int pkgId, int originId,
                                       int destId) {
  printFormattedTime(time);
  std::cout << " pacote " << std::setw(3) << pkgId << " em transito de "
            << std::setw(3) << originId << " para " << std::setw(3) << destId
            << std::endl;
}

void OutputLogger::logPackageDelivered(int time, int pkgId, int whId) {
  printFormattedTime(time);
  std::cout << " pacote " << std::setw(3) << pkgId << " entregue em "
            << std::setw(3) << whId << std::endl;
}

void OutputLogger::logPackageRemoved(int time, int pkgId, int whId,
                                     int sectionId) {
  printFormattedTime(time);
  std::cout << " pacote " << std::setw(3) << pkgId << " removido de "
            << std::setw(3) << whId << " na secao " << std::setw(3) << sectionId
            << std::endl;
}
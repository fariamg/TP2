#pragma once

//@ Funções de logging para eventos da simulação
void printFormattedTime(int time);
void logPackageStored(int time, int pkgId, int whId, int sectionId);
void logPackageInTransit(int time, int pkgId, int originId, int destId);
void logPackageDelivered(int time, int pkgId, int whId);
void logPackageRemoved(int time, int pkgId, int whId, int sectionId);
void logPackageRestored(int time, int pkgId, int whId, int sectionId);
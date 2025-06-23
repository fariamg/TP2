#include "../../include/domains/WarehouseExtended.h"
#include "../../include/domains/PackageExtended.h"
#include "../../include/utils/Logger.h"
#include <iostream>

WarehouseExtended::WarehouseExtended(int ID, int totalWarehouses, int capacity, int weightCapacity)
    : ID(ID), totalWarehouses(totalWarehouses), totalCapacity(capacity), totalOccupancy(0), totalWeightCapacity(weightCapacity), totalCurrentWeight(0), secondaryStorageId(-1),
      hasSecondaryStorage(false) {

    sections = new SectionExtended[totalWarehouses];

    // Inicializa seções com capacidade distribuída igualmente
    int sectionCapacity = capacity / totalWarehouses;
    int sectionWeightCapacity = weightCapacity / totalWarehouses;

    for (int i = 0; i < totalWarehouses; ++i) {
        sections[i].destinationId = i;
        sections[i].maxCapacity = sectionCapacity;
        sections[i].weightCapacity = sectionWeightCapacity;
        sections[i].currentOccupancy = 0;
        sections[i].currentWeight = 0;
    }
}

WarehouseExtended::~WarehouseExtended() {
    delete[] sections;
}

int WarehouseExtended::getId() const noexcept {
    return ID;
}

int WarehouseExtended::getTotalCapacity() const noexcept {
    return totalCapacity;
}

int WarehouseExtended::getTotalOccupancy() const noexcept {
    return totalOccupancy;
}

int WarehouseExtended::getTotalWeightCapacity() const noexcept {
    return totalWeightCapacity;
}

int WarehouseExtended::getTotalCurrentWeight() const noexcept {
    return totalCurrentWeight;
}

double WarehouseExtended::getUtilizationPercentage() const noexcept {
    if (totalCapacity == 0)
        return 0.0;
    return (static_cast<double>(totalOccupancy) / totalCapacity) * 100.0;
}

int WarehouseExtended::getSectionSize(int sectionId) const noexcept {
    if (sectionId >= 0 && sectionId < totalWarehouses) {
        return sections[sectionId].stack.getCurrentSize();
    }
    return 0;
}

int WarehouseExtended::getSectionAvailableCapacity(int sectionId) const noexcept {
    if (sectionId >= 0 && sectionId < totalWarehouses) {
        return sections[sectionId].maxCapacity - sections[sectionId].currentOccupancy;
    }
    return 0;
}

int WarehouseExtended::getSectionAvailableWeight(int sectionId) const noexcept {
    if (sectionId >= 0 && sectionId < totalWarehouses) {
        return sections[sectionId].weightCapacity - sections[sectionId].currentWeight;
    }
    return 0;
}

bool WarehouseExtended::canStorePackage(PackageExtended* pkg, int destinationId) const {
    if (!pkg || destinationId < 0 || destinationId >= totalWarehouses) {
        return false;
    }

    const SectionExtended& section = sections[destinationId];
    int spaceRequired = pkg->getStorageSpaceRequired();
    int weightRequired = pkg->getWeight();

    return (section.currentOccupancy + spaceRequired <= section.maxCapacity) && (section.currentWeight + weightRequired <= section.weightCapacity);
}

bool WarehouseExtended::storePackage(PackageExtended* pkg, int destinationId) {
    if (!canStorePackage(pkg, destinationId)) {
        // Tenta armazenar na fila de espera e retorna o resultado
        return storeInWaitingQueue(pkg, destinationId);
    }

    SectionExtended& section = sections[destinationId];
    section.stack.push(pkg);

    int spaceRequired = pkg->getStorageSpaceRequired();
    int weightRequired = pkg->getWeight();

    section.currentOccupancy += spaceRequired;
    section.currentWeight += weightRequired;
    totalOccupancy += spaceRequired;
    totalCurrentWeight += weightRequired;

    return true;
}

bool WarehouseExtended::storeInWaitingQueue(PackageExtended* pkg, int destinationId) {
    if (destinationId < 0 || destinationId >= totalWarehouses) {
        return false;
    }

    SectionExtended& section = sections[destinationId];
    section.waitingQueue.enqueue(pkg);

    // Log overflow event
    logPackageWaitingDueToCapacity(0, pkg->getId(), ID, destinationId); // Timer would be passed from caller

    return true;
}

void WarehouseExtended::processWaitingQueue(int destinationId) {
    if (destinationId < 0 || destinationId >= totalWarehouses) {
        return;
    }

    SectionExtended& section = sections[destinationId];

    while (!section.waitingQueue.isEmpty()) {
        PackageExtended* pkg = static_cast<PackageExtended*>(section.waitingQueue.peek());

        if (canStorePackage(pkg, destinationId)) {
            section.waitingQueue.dequeue();
            // Directly store in main storage without calling storePackage to avoid recursion
            section.stack.push(pkg);

            int spaceRequired = pkg->getStorageSpaceRequired();
            int weightRequired = pkg->getWeight();

            section.currentOccupancy += spaceRequired;
            section.currentWeight += weightRequired;
            totalOccupancy += spaceRequired;
            totalCurrentWeight += weightRequired;
        } else {
            break; // Não pode armazenar mais pacotes
        }
    }
}

PackageExtended* WarehouseExtended::retrievePackage(int destinationId) {
    if (destinationId < 0 || destinationId >= totalWarehouses) {
        return nullptr;
    }

    SectionExtended& section = sections[destinationId];
    PackageExtended* pkg = static_cast<PackageExtended*>(section.stack.pop());

    if (pkg) {
        int spaceFreed = pkg->getStorageSpaceRequired();
        int weightFreed = pkg->getWeight();

        section.currentOccupancy -= spaceFreed;
        section.currentWeight -= weightFreed;
        totalOccupancy -= spaceFreed;
        totalCurrentWeight -= weightFreed;

        // Processa fila de espera quando há espaço disponível
        processWaitingQueue(destinationId);
    }

    return pkg;
}

const PackageExtended* WarehouseExtended::peekAtSection(int destinationId) const {
    if (destinationId >= 0 && destinationId < totalWarehouses) {
        return static_cast<const PackageExtended*>(sections[destinationId].stack.peek());
    }
    return nullptr;
}

bool WarehouseExtended::isSectionEmpty(int destinationId) const {
    if (destinationId >= 0 && destinationId < totalWarehouses) {
        return sections[destinationId].stack.isEmpty();
    }
    return true;
}

bool WarehouseExtended::isSectionOverCapacity(int destinationId) const {
    if (destinationId >= 0 && destinationId < totalWarehouses) {
        const SectionExtended& section = sections[destinationId];
        return section.currentOccupancy > section.maxCapacity || section.currentWeight > section.weightCapacity;
    }
    return false;
}

void WarehouseExtended::setSecondaryStorage(int warehouseId) {
    secondaryStorageId = warehouseId;
    hasSecondaryStorage = true;
}

bool WarehouseExtended::transferToSecondaryStorage(PackageExtended* pkg, int destinationId, WarehouseExtended** warehouses) {
    if (!hasSecondaryStorage || secondaryStorageId < 0 || !pkg || !warehouses) {
        return false;
    }

    WarehouseExtended* secondaryWarehouse = warehouses[secondaryStorageId];
    if (secondaryWarehouse && secondaryWarehouse->canStorePackage(pkg, destinationId)) {
        return secondaryWarehouse->storePackage(pkg, destinationId);
    }

    return false;
}

int WarehouseExtended::getWaitingQueueSize(int destinationId) const noexcept {
    if (destinationId >= 0 && destinationId < totalWarehouses) {
        return sections[destinationId].waitingQueue.getSize();
    }
    return 0;
}

void WarehouseExtended::logPackageWaitingDueToCapacity(int time, int pkgId, int whId, int sectionId) {
    std::cout << "[" << time << "] Pacote " << pkgId << " aguardando capacidade no armazém " << whId << " seção " << sectionId << std::endl;
}

void WarehouseExtended::optimizeCapacity() {
    // Implementation for capacity optimization could be added here
    // For now, this is a placeholder
}
#include "../include/core/Scheduler.h"
#include "../include/ui/OutputLogger.h"
#include <iostream>

// TODO: MELHORAR ESSA MODULARIZAÇÃO (RESPONSABILIDADE ÚNICA)

Scheduler::Scheduler(const IO::ConfigData& configData, int maxEvents) : eventsHeap(maxEvents), timer() {
    this->transportCapacity = configData.transportCapacity;
    this->transportLatency = configData.transportLatency;
    this->transportInterval = configData.transportInterval;
    this->removalCost = configData.removalCost;

    for (int i = 0; i < configData.numPackages; ++i) {
        Package* package = configData.packages[i];
        if (package != nullptr) {
            Event* event = new Event(package->getPostTime(), package);
            this->addEvent(event);
        }
    }

    timer.setTime(eventsHeap.peekMax()->time); 

    initialStoragesInWarehouse(configData.warehouses);

    updatePreviousStorageTime(configData.packages, configData.numPackages, timer.getTime());
}

Event* Scheduler::getNextEvent() {
    return this->eventsHeap.peekMin();
}

int Scheduler::getTotalEvents() const noexcept {
    return this->eventsHeap.getCurrentSize();
}

int Scheduler::getCurrentTime() const noexcept {
    return this->timer.getTime();
}
void Scheduler::addEvent(Event* event) {
    this->eventsHeap.insert(event);
}

Event* Scheduler::removeNextEvent() {
    if (this->eventsHeap.isEmpty()) {
        throw std::out_of_range("No events to remove");
    }
    return this->eventsHeap.extractMin();
}

void Scheduler::advanceTimer(int timeToAdd) {
    this->timer.addTime(timeToAdd);
}

bool Scheduler::isEmpty() const noexcept {
    return this->eventsHeap.isEmpty();
}

void Scheduler::printEvents() {
    eventsHeap.printHeap();
}

void Scheduler::updatePreviousStorageTime(Package** packages, int lastIndex, int maxTime) {
    for (int i = 0; i < lastIndex; ++i) {
        if (packages[i] != nullptr) {
            packages[i]->setTimeStored(maxTime - packages[i]->getPostTime());
        }
    }
}

void Scheduler::initialStoragesInWarehouse(Warehouse** warehouses) noexcept {
    std::cout << getTotalEvents() << " initial storage events to process." << std::endl;
    int numEventsToProcess = getTotalEvents();

    for (int i = 0; i < numEventsToProcess; ++i) {
        Event* event = removeNextEvent();

        Package* package = event->package;
        package->getRoute().removeFront();
        int originId = package->getInitialOrigin();

        int nextWarehouseInSectionId = package->getRoute().peekFront();

        warehouses[originId]->storePackage(package, nextWarehouseInSectionId);

        package->getRoute().removeFront();

        package->setCurrentLocation(originId);
        package->setState(PackageState::STORED);
        OutputLogger::logPackageStored(package->getPostTime(), package->getId(), originId, nextWarehouseInSectionId);
    }
}

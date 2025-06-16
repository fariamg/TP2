#include "../include/core/Scheduler.h"

Scheduler::Scheduler(int maxEvents, const IO::ConfigData& configData) : eventsHeap(maxEvents), timer() {
    this->transportCapacity = configData.transportCapacity;
    this->transportLatency = configData.transportLatency;
    this->transportInterval = configData.transportInterval;
    this->removalCost = configData.removalCost;
}

int Scheduler::getTotalEvents() const noexcept {
    return this->eventsHeap.getCurrentSize();
}


void Scheduler::addEvent(Event* event) {
    this->eventsHeap.insert(event);
}

Event* Scheduler::getNextEvent() {
    return this->eventsHeap.peekMin();
}

void Scheduler::removeNextEvent() {
    if (this->eventsHeap.isEmpty()) {
        throw std::out_of_range("No events to remove");
    }
    this->eventsHeap.extractMin();
}

void Scheduler::advanceTimer(int timeToAdd) {
    this->timer.addTime(timeToAdd);
}

int Scheduler::getCurrentTime() const noexcept {
    return this->timer.getTime();
}

bool Scheduler::isEmpty() const noexcept {
    return this->eventsHeap.isEmpty();
}

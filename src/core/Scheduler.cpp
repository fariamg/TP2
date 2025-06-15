#include "../include/core/Scheduler.h"

Scheduler::Scheduler(int maxEvents) : eventsHeap(maxEvents), timer() {}

void Scheduler::addEvent(Event *event) {
    this->eventsHeap.insert(event);
}

Event *Scheduler::getNextEvent() {
    return this->eventsHeap.extractMin();
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
#include "../include/core/Scheduler.h"

Scheduler::Scheduler(int maxEvents = MAX_EVENTS) : eventsHeap(maxEvents), timer() {}

int Scheduler::getTotalEvents() const noexcept {
    return this->totalEvents;
}

void Scheduler::incrementTotalEvents() noexcept {
    this->totalEvents++;
}

void Scheduler::addEvent(Event *event) {
    this->eventsHeap.insert(event);
    this->incrementTotalEvents();
}

Event *Scheduler::getNextEvent() {
    return this->eventsHeap.peekMin();
}

void *Scheduler::removeNextEvent() {
    if (this->eventsHeap.isEmpty()) {
        return nullptr;
    }
    this->totalEvents--;
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

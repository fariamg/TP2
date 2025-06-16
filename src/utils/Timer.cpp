#include "../include/utils/Timer.h"
#include <stdexcept>

Timer::Timer() : time(0) {}

int Timer::getTime() const noexcept {
    return time;
}

void Timer::addTime(int time) {
    if (time < 0) {
        throw std::invalid_argument("Time cannot be negative");
    }
    this->time += time;
}
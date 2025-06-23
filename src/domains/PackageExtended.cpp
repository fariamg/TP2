#include "../../include/domains/PackageExtended.h"
#include <cmath>

PackageExtended::PackageExtended(int ID, int initialOrigin, int finalDestination, int postTime, int weight, bool specialHandling)
    : Package(ID, initialOrigin, finalDestination, postTime), weight(weight), lastRouteCalculation(0), requiresSpecialHandling(specialHandling) {
    this->weightClass = determineWeightClass(weight);
}

int PackageExtended::getWeight() const noexcept {
    return weight;
}

PackageWeightClass PackageExtended::getWeightClass() const noexcept {
    return weightClass;
}

bool PackageExtended::getRequiresSpecialHandling() const noexcept {
    return requiresSpecialHandling;
}

const LinkedList& PackageExtended::getRouteHistory() const noexcept {
    return routeHistory;
}

int PackageExtended::getLastRouteCalculation() const noexcept {
    return lastRouteCalculation;
}

void PackageExtended::addRouteToHistory(const LinkedList& route) {
    // Add route to history - simplified implementation
    // In practice, you might want to limit the history size
    routeHistory = route;
}

void PackageExtended::updateLastRouteCalculation(int time) {
    lastRouteCalculation = time;
}

double PackageExtended::getWeightImpactFactor() const noexcept {
    // Weight impact factor for transport calculations
    // Heavier packages take more time/resources
    switch (weightClass) {
    case LIGHT:
        return 1.0;
    case MEDIUM:
        return 1.2;
    case HEAVY:
        return 1.5;
    case EXTRA_HEAVY:
        return 2.0;
    default:
        return 1.0;
    }
}

int PackageExtended::getStorageSpaceRequired() const noexcept {
    // Storage space required based on weight class
    // Heavier packages require more space
    switch (weightClass) {
    case LIGHT:
        return 1;
    case MEDIUM:
        return 2;
    case HEAVY:
        return 3;
    case EXTRA_HEAVY:
        return 5;
    default:
        return 1;
    }
}

PackageWeightClass PackageExtended::determineWeightClass(int weight) {
    if (weight <= 1)
        return LIGHT;
    else if (weight <= 3)
        return MEDIUM;
    else if (weight <= 5)
        return HEAVY;
    else
        return EXTRA_HEAVY;
}
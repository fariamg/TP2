#include "../core/Package.h"

Package::Package()
    : timeStored(0), timeInTransit(0), expectedStorageTime(0),
      state(PackageState::NOT_POSTED) {}

std::string Package::getSender() const noexcept { return sender; }

std::string Package::getRecipient() const noexcept { return recipient; }

std::string Package::getType() const noexcept { return type; }

Warehouse Package::getOriginWarehouse() const noexcept {
  return originWarehouse;
}

Warehouse Package::getDestinationWarehouse() const noexcept {
  return destinationWarehouse;
}

Date Package::getPostDate() const noexcept { return postDate; }

int Package::getTimeStored() const noexcept { return timeStored; }

int Package::getTimeInTransit() const noexcept { return timeInTransit; }

int Package::getExpectedStorageTime() const noexcept {
  return expectedStorageTime;
}

void Package::setTimeInTransit(int time) { timeInTransit = time; }

void Package::setTimeStored(int time) { timeStored = time; }

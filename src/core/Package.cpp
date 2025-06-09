#include "../core/Package.h"

//* O estado inicial sempre é NOT_POSTED, pois o pacote ainda não foi postado
Package::Package(int id)
    : timeStored(0), timeInTransit(0), expectedStorageTime(0), id (id),
      state(PackageState::NOT_POSTED) {}

int Package::getId() const noexcept { return this->id; }

//std::string Package::getSender() const noexcept { return this->sender; }

//std::string Package::getRecipient() const noexcept { return this->recipient; }

Warehouse Package::getOriginWarehouse() const noexcept {
  return this->originWarehouse;
}

Warehouse Package::getDestinationWarehouse() const noexcept {
  return this->destinationWarehouse;
}

int Package::getPostDate() const noexcept { return this->postDate; }

int Package::getTimeStored() const noexcept { return this->timeStored; }

int Package::getTimeInTransit() const noexcept { return this->timeInTransit; }

int Package::getExpectedStorageTime() const noexcept {
  return this->expectedStorageTime;
}

PackageState Package::getState() const noexcept { return this->state; }

void Package::setTimeInTransit(int time) { this->timeInTransit = time; }

void Package::setTimeStored(int time) { this->timeStored = time; }

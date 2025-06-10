#include "Package.h"
//* O estado inicial sempre é NOT_POSTED, pois o pacote ainda não foi postado
Package::Package(int ID, int initialOrigin, int finalDestination, int postDate,
                 int expectedStorageTime, int totalWarehouses)
    : ID(ID), initialOrigin(initialOrigin), finalDestination(finalDestination),
      postDate(postDate), timeStored(0), timeInTransit(0),
      expectedStorageTime(expectedStorageTime),
      state(PackageState::NOT_POSTED) {
  this->route = new Warehouse[totalWarehouses];
}

int Package::getId() const noexcept { return this->ID; }

// std::string Package::getSender() const noexcept { return this->sender; }

// std::string Package::getRecipient() const noexcept { return this->recipient;
// }

int Package::getInitialOrigin() const noexcept {
  return this->initialOrigin;
}

int Package::getFinalDestination() const noexcept {
  return this->finalDestination;
}

int Package::getPostDate() const noexcept { return this->postDate; }

int Package::getTimeStored() const noexcept { return this->timeStored; }

int Package::getTimeInTransit() const noexcept { return this->timeInTransit; }

int Package::getExpectedStorageTime() const noexcept {
  return this->expectedStorageTime;
}

PackageState Package::getState() const noexcept { return this->state; }

int* Package::getRoute() const noexcept {
  int* routeIds = new int[sizeof(this->route) / sizeof(Warehouse)];
  for (size_t i = 0; i < sizeof(this->route) / sizeof(Warehouse); ++i) {
    routeIds[i] = this->route[i].getId();
  }
  return routeIds;
}

void Package::setTimeInTransit(int time) { this->timeInTransit = time; }

void Package::setTimeStored(int time) { this->timeStored = time; }

#include "../include/domains/Package.h"

//* O estado inicial sempre é NOT_POSTED, pois o pacote ainda não foi postado
Package::Package(int ID, int initialOrigin, int finalDestination, int postDate)
    : ID(ID), initialOrigin(initialOrigin), finalDestination(finalDestination),
      postDate(postDate), timeStored(0), timeInTransit(0),
      state(PackageState::NOT_POSTED) {}

int Package::getId() const noexcept { return this->ID; }

// std::string Package::getSender() const noexcept { return this->sender; }

// std::string Package::getRecipient() const noexcept { return this->recipient;
// }

int Package::getInitialOrigin() const noexcept { return this->initialOrigin; }

int Package::getFinalDestination() const noexcept {
  return this->finalDestination;
}

int Package::getPostDate() const noexcept { return this->postDate; }

int Package::getTimeStored() const noexcept { return this->timeStored; }

int Package::getTimeInTransit() const noexcept { return this->timeInTransit; }

int Package::getCurrentLocation() const noexcept {
  return this->currentLocation;
}

PackageState Package::getState() const noexcept { return this->state; }

LinkedList<int> Package::getRoute() const noexcept { return this->route; }

void Package::setTimeInTransit(int time) { this->timeInTransit = time; }

void Package::setCurrentLocation(int location) {
  this->currentLocation = location;
}

void Package::setTimeStored(int time) { this->timeStored = time; }

void Package::setRoute(const LinkedList<int> &newRoute) noexcept {
  this->route = newRoute;
}

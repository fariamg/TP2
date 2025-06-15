#include "../include/domains/Warehouse.h"

Warehouse::Warehouse(int ID, int totalWarehouses)
    : ID(ID), totalWarehouses(totalWarehouses) {
  this->sessions = new Stack[totalWarehouses];
}

Warehouse::~Warehouse() { delete[] this->sessions; }

int Warehouse::getId() const noexcept { return this->ID; }

void Warehouse::storePackage(const Package *pkg, int destinationId) {
  if (destinationId >= 0 && destinationId < this->totalWarehouses) {
    this->sessions[destinationId].push(pkg);
  }
}

const Package *Warehouse::retrievePackage(int destinationId) {
  if (destinationId >= 0 && destinationId < this->totalWarehouses) {
    return this->sessions[destinationId].pop();
  }
  return nullptr;
}

const Package *Warehouse::peekAtSection(int destinationId) const {
  if (destinationId >= 0 && destinationId < this->totalWarehouses) {
    return this->sessions[destinationId].peek();
  }
  return nullptr;
}

bool Warehouse::isSectionEmpty(int destinationId) const {
  if (destinationId >= 0 && destinationId < this->totalWarehouses) {
    return this->sessions[destinationId].isEmpty();
  }
  return true;
}
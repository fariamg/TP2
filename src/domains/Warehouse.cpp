#include "Warehouse.h"

Warehouse::Warehouse(int ID) : ID(ID) {}

int Warehouse::getId() const noexcept { return this->ID; }

Stack Warehouse::getPackages() const noexcept { return this->packages; }
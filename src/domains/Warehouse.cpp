#include "../include/domains/Warehouse.h"

Warehouse::Warehouse(int ID, int totalWarehouses) : ID(ID), totalWarehouses(totalWarehouses) {
    this->sessions = new Session[totalWarehouses];

    for (int i = 0; i < totalWarehouses; ++i) {
        this->sessions[i].destinationId = i;
    }
}

Warehouse::~Warehouse() {
    delete[] this->sessions;
}

int Warehouse::getId() const noexcept {
    return this->ID;
}

void Warehouse::storePackage(const Package* pkg, int destinationId) {
    if (destinationId >= 0 && destinationId < this->totalWarehouses) {
        this->sessions[destinationId].stack.push(pkg);
    }
}

const Package* Warehouse::retrievePackage(int destinationId) {
    if (destinationId >= 0 && destinationId < this->totalWarehouses) {
        return this->sessions[destinationId].stack.pop();
    }
    return nullptr;
}

const Package* Warehouse::peekAtSection(int destinationId) const {
    if (destinationId >= 0 && destinationId < this->totalWarehouses) {
        return this->sessions[destinationId].stack.peek();
    }
    return nullptr;
}

bool Warehouse::isSectionEmpty(int destinationId) const {
    if (destinationId >= 0 && destinationId < this->totalWarehouses) {
        return this->sessions[destinationId].stack.isEmpty();
    }
    return true;
}
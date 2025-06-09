#include "Event.h"

Event::Event(int time, Package *package)
    : type(EventType::PACKAGE_ARRIVAL), time(time), package(package),
      originWarehouseId(-1), destinationWarehouseId(-1) {}


Event::Event(int time, int originWarehouseId, int destinationWarehouseId)
    : type(EventType::TRANSPORT), time(time), package(nullptr),
      originWarehouseId(originWarehouseId), destinationWarehouseId(destinationWarehouseId) {}


//TODO: se o tempo for igual, usar qual critério de desempate?
bool Event::operator<(const Event &other) const noexcept {
    if (this->time < other.time) {
        return true;
    }
    if (this->time > other.time) {
        return false;
    }

    if (this->type == EventType::PACKAGE_ARRIVAL && other.type == EventType::TRANSPORT) {
        return true; 
    }
    if (this->type == EventType::TRANSPORT && other.type == EventType::PACKAGE_ARRIVAL) {
        return false;
    }
    
    if (this->type == EventType::PACKAGE_ARRIVAL) {
        return this->package->getId() < other.package->getId();
    } else { 
        if (this->originWarehouseId != other.originWarehouseId) {
            return this->originWarehouseId < other.originWarehouseId;
        }
        return this->destinationWarehouseId < other.destinationWarehouseId;
    }
}
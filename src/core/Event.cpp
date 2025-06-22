#include "../include/core/Event.h"

Event::Event(int time, Package* package) : type(EventType::PACKAGE_ARRIVAL), time(time), package(package), originWarehouseId(-1), destinationSectionId(-1) {}

Event::Event(int time, int originWarehouseId, int destinationSectionId)
    : type(EventType::LEAVE_TRANSPORT), time(time), package(nullptr), originWarehouseId(originWarehouseId), destinationSectionId(destinationSectionId) {}

bool Event::operator<(const Event& other) const noexcept {
    //* Compara os eventos com base no tempo, tipo e ID do pacote ou armazém
    if (this->time != other.time) {
        return this->time < other.time;
    }

    if (this->type != other.type) {
        return this->type < other.type;
    }

    if (this->type == EventType::PACKAGE_ARRIVAL) {
        return this->package->getId() < other.package->getId();
    } else { 
        if (this->originWarehouseId != other.originWarehouseId) {
            return this->originWarehouseId < other.originWarehouseId;
        }
        return this->destinationSectionId < other.destinationSectionId;
    }
}
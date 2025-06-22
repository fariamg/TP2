#include "../include/core/Event.h"

Event::Event(int time, Package* package) : type(EventType::PACKAGE_ARRIVAL), time(time), package(package), originWarehouseId(-1), destinationSectionId(-1) {}

Event::Event(int time, int originWarehouseId, int destinationSectionId)
    : type(EventType::LEAVE_TRANSPORT), time(time), package(nullptr), originWarehouseId(originWarehouseId), destinationSectionId(destinationSectionId) {}

// TODO: se o tempo for igual, usar qual critério de desempate?
bool Event::operator<(const Event& other) const noexcept {
    // Critério 1: Tempo do evento. O menor tempo tem maior prioridade.
    if (this->time != other.time) {
        return this->time < other.time;
    }

    // O tempo é igual. Passamos para o critério de desempate 2: Tipo do Evento.
    // Usamos a ordem definida no 'enum class'. PACKAGE_ARRIVAL (0) < LEAVE_TRANSPORT (1).
    if (this->type != other.type) {
        return this->type < other.type;
    }

    // O tempo e o tipo são iguais. Isso só pode acontecer se ambos os eventos
    // forem do mesmo tipo. Usamos o critério de desempate 3: Dados do Evento.
    if (this->type == EventType::PACKAGE_ARRIVAL) {
        // Se ambos são chegadas de pacote, desempata pelo menor ID do pacote.
        return this->package->getId() < other.package->getId();
    } else { // type == EventType::LEAVE_TRANSPORT
        // Se ambos são saídas de transporte, desempata primeiro pelo armazém de origem...
        if (this->originWarehouseId != other.originWarehouseId) {
            return this->originWarehouseId < other.originWarehouseId;
        }
        // ...e se a origem for a mesma, desempata pela seção de destino.
        return this->destinationSectionId < other.destinationSectionId;
    }
}
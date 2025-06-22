#include "../include/domains/Package.h"

//* O estado inicial sempre é NOT_POSTED, pois o pacote ainda não foi postado
Package::Package(int ID, int initialOrigin, int finalDestination, int postTime)
    : ID(ID), initialOrigin(initialOrigin), finalDestination(finalDestination), postTime(postTime), timeStored(0), timeInTransit(0), state(PackageState::NOT_POSTED) {}

int Package::getId() const noexcept {
    return this->ID;
}

int Package::getInitialOrigin() const noexcept {
    return this->initialOrigin;
}

int Package::getFinalDestination() const noexcept {
    return this->finalDestination;
}

int Package::getPostTime() const noexcept {
    return this->postTime;
}

int Package::getTimeStored() const noexcept {
    return this->timeStored;
}

int Package::getTimeInTransit() const noexcept {
    return this->timeInTransit;
}

int Package::getCurrentLocation() const noexcept {
    return this->currentLocation;
}

PackageState Package::getState() const noexcept {
    return this->state;
}

LinkedList& Package::getRoute() noexcept {
    return this->route;
}

void Package::setTimeInTransit(int time) {
    this->timeInTransit = time;
}

void Package::setCurrentLocation(int location) {
    this->currentLocation = location;
}

void Package::setTimeStored(int time) {
    this->timeStored = time;
}

void Package::setRoute(LinkedList&& newRoute) noexcept {
    this->route = std::move(newRoute);
}

void Package::setState(PackageState newState) noexcept {
    this->state = newState;
}

namespace Routing {
LinkedList calculateOptimalRoute(int originId, int destinationId, const Graph& graph) {
    int numWarehouses = graph.getNumWarehouses();
    LinkedList finalRoute;

    if (originId == destinationId) {
        finalRoute.addFront(originId);
        return finalRoute;
    }

    bool* visited = new bool[numWarehouses];
    int* predecessor = new int[numWarehouses];
    for (int i = 0; i < numWarehouses; ++i) {
        visited[i] = false;
        predecessor[i] = -1;
    }

    LinkedList bfsQueue;

    visited[originId] = true;
    bfsQueue.addBack(originId);

    bool pathFound = false;
    while (!bfsQueue.isEmpty()) {
        int currentWarehouse = bfsQueue.peekFront();
        bfsQueue.removeFront();

        if (currentWarehouse == destinationId) {
            pathFound = true;
            break;
        }

        LinkedList neighbors = graph.getNeighbors(currentWarehouse);

        while (!neighbors.isEmpty()) {
            int neighbor = neighbors.peekFront();
            neighbors.removeFront();

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                predecessor[neighbor] = currentWarehouse;
                bfsQueue.addBack(neighbor);
            }
        }
    }

    if (pathFound) {
        LinkedList reversedPath;
        int crawl = destinationId;

        while (crawl != -1) {
            reversedPath.addFront(crawl);
            crawl = predecessor[crawl];
        }

        while (!reversedPath.isEmpty()) {
            int nodeId = reversedPath.peekFront();
            reversedPath.removeFront();
            finalRoute.addBack(nodeId);
        }
    }

    delete[] predecessor;

    return finalRoute;
}
} // namespace Routing
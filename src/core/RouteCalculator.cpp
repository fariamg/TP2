#include "../include/core/RouteCalculator.h"
#include "../include/dataStructures/Graph.h"

namespace Routing {
LinkedList<int> calculateShortestRoute(int originId, int destinationId,
                                       const Graph &graph) {
  int numWarehouses = graph.getNumWarehouses();
  LinkedList<int> finalRoute;

  if (originId == destinationId) {
    finalRoute.addFront(originId);
    return finalRoute;
  }

  bool *visited = new bool[numWarehouses];
  int *predecessor = new int[numWarehouses];
  for (int i = 0; i < numWarehouses; ++i) {
    visited[i] = false;
    predecessor[i] = -1;
  }

  LinkedList<int> bfsQueue;

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

    LinkedList<int> neighbors = graph.getNeighbors(currentWarehouse);

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
    LinkedList<int> reversedPath;
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
} 
#pragma once

#include "Graph.h"
#include "Warehouse.h"
//TODO: usar DIJKSTRA para levar em consideração o peso entre as arestas (ponto extra)
//@ Função livre que calcula a rota ótima entre dois armazéns
LinkedList<int> calculateOptimalRoute(int origninID, int destinationID, const Graph& graph);
#pragma once

#include "dataStructures/Graph.h"
#include "domains/Warehouse.h"
// TODO: usar DIJKSTRA para levar em consideração o peso entre as arestas (ponto
// extra)
//@ Função livre que calcula a rota ótima entre dois armazéns

namespace Routing {

//@ Função que calcula a rota ótima entre dois armazéns
//@ @param originID ID do armazém de origem
//@ @param destinationID ID do armazém de destino
//@ @param graph Grafo que representa os armazéns e suas conexões
LinkedList calculateOptimalRoute(int origninID, int destinationID,
                                 const Graph &graph);
} // namespace Routing
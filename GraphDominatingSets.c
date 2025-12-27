//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// Joaquim Madeira - Nov 2025
//
// GraphDominatingSets - Computing Vertex Dominating Sets for UNDIRECTED graphs
//

// Student Name : David Cálix
// Student Number : 125043
// Student Name : Diogo Ruivo
// Student Number : 126498

/*** COMPLETE THE GraphIsDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinWeightDominatingSet FUNCTION ***/

#include "GraphDominatingSets.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IndicesSet.h"
#include "instrumentation.h"

//
// TO BE COMPLETED
//
// Check if the given set is a dominating set for the graph
// Return 1 if true, or 0 otherwise
//
int GraphIsDominatingSet(const Graph* g, IndicesSet* vertSet) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);
  assert(IndicesSetIsEmpty(vertSet) == 0);

  // Vamos percorrer todos os vértices do grafo
  IndicesSet* allVertices = GraphGetSetVertices(g);
  int v = IndicesSetGetFirstElem(allVertices);

  while (v != -1) {
    // Se o vértice v NÃO está no conjunto dominante...
    if (!IndicesSetContains(vertSet, v)) {
      // ...temos de garantir que pelo menos um vizinho dele ESTÁ.
      IndicesSet* neighbors = GraphGetSetAdjacentsTo(g, v);
      int isCovered = 0;

      int neighbor = IndicesSetGetFirstElem(neighbors);
      while (neighbor != -1) {
        if (IndicesSetContains(vertSet, neighbor)) {
          isCovered = 1;
          break; // Já encontrámos um vizinho no set, v está coberto!
        }
        neighbor = IndicesSetGetNextElem(neighbors);
      }

      IndicesSetDestroy(&neighbors);

      // Se nenhum vizinho estiver no set, então vertSet NÃO é dominante.
      if (!isCovered) {
        IndicesSetDestroy(&allVertices);
        return 0;
      }
    }
    v = IndicesSetGetNextElem(allVertices);
  }

  IndicesSetDestroy(&allVertices);
  return 1; // Passou em todos os testes
}

//
// TO BE COMPLETED
//
// Compute a MIN VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the/a dominating set
//
IndicesSet* GraphComputeMinDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  unsigned int range = GraphGetVertexRange(g);
  IndicesSet* currentSet = IndicesSetCreateEmpty(range);
  IndicesSet* bestSet = NULL;

  // Loop para testar todos os subconjuntos possíveis
  do {
    // Ignorar conjunto vazio (a menos que o grafo seja vazio, mas o assert protege)
    if (IndicesSetIsEmpty(currentSet)) continue;

    // Se este conjunto domina o grafo...
    if (GraphIsDominatingSet(g, currentSet)) {
      // ...e se for melhor (menor) que o melhor que já tínhamos...
      if (bestSet == NULL || IndicesSetGetNumElems(currentSet) < IndicesSetGetNumElems(bestSet)) {
        // Atualizamos o bestSet
        if (bestSet != NULL) IndicesSetDestroy(&bestSet);
        bestSet = IndicesSetCreateCopy(currentSet);
      }
    }
  } while (IndicesSetNextSubset(currentSet));

  IndicesSetDestroy(&currentSet);

  if (bestSet == NULL) {
    return IndicesSetCreateEmpty(range); // Fallback
  }

  return bestSet;
}

//
// TO BE COMPLETED
//
// Compute a MIN WEIGHT VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the dominating set
//
IndicesSet* GraphComputeMinWeightDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  // 1. Calcular os pesos de todos os vértices uma única vez
  double* weights = GraphComputeVertexWeights(g);
  unsigned int range = GraphGetVertexRange(g);

  IndicesSet* currentSet = IndicesSetCreateEmpty(range);
  IndicesSet* bestSet = NULL;
  double minWeight = -1.0; // -1 indica que ainda não encontrámos nenhum válido

  do {
    if (IndicesSetIsEmpty(currentSet)) continue;

    // 2. Calcular o peso deste conjunto candidato
    double currentWeight = 0.0;
    int v = IndicesSetGetFirstElem(currentSet);
    while (v != -1) {
      if (weights[v] != -1.0) {
        currentWeight += weights[v];
      }
      v = IndicesSetGetNextElem(currentSet);
    }

    // OTIMIZAÇÃO: Se já temos uma solução melhor ou igual, saltamos esta iteração
    // (Poupamos a chamada cara a GraphIsDominatingSet)
    if (minWeight != -1.0 && currentWeight >= minWeight) {
      continue;
    }

    // 3. Se passou no teste do peso, verificamos se domina
    if (GraphIsDominatingSet(g, currentSet)) {
      if (bestSet == NULL || currentWeight < minWeight) {
        if (bestSet != NULL) IndicesSetDestroy(&bestSet);
        bestSet = IndicesSetCreateCopy(currentSet);
        minWeight = currentWeight;
      }
    }

  } while (IndicesSetNextSubset(currentSet));

  free(weights); // Não esquecer de limpar a memória do array de pesos
  IndicesSetDestroy(&currentSet);

  if (bestSet == NULL) return IndicesSetCreateEmpty(range);

  return bestSet;
}

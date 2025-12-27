//
// Benchmark.c - Ferramenta Auxiliar para Análise de Complexidade
//
// ESTE FICHEIRO NÃO FAZ PARTE DO ESQUELETO BASE DO PROJETO.
// Foi criado para gerar os dados experimentais necessários para o
// Relatório de Análise de Complexidade.
//
// O programa gera grafos aleatórios de tamanho crescente e mede
// o tempo de execução dos algoritmos de força bruta para encontrar
// conjuntos dominantes.
//
// Compilar com:
// gcc -Wall -Wextra -O2 -o Benchmark Benchmark.c Graph.c GraphDominatingSets.c IndicesSet.c SortedList.c instrumentation.c
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Graph.h"
#include "GraphDominatingSets.h"
#include "instrumentation.h"

int main(void) {
  // Inicializar o gerador de números aleatórios
  srand(time(NULL));

  // Cabeçalho da tabela para facilitar copiar para Excel
  printf("Vertices(N)\tTempo_MinDominatingSet(s)\tTempo_MinWeightDominatingSet(s)\n");

  // ATENÇÃO: Como o algoritmo é exponencial (2^N), tamanhos acima de 22-23
  // podem demorar MUITO tempo. Começamos com grafos pequenos.
  for (int n = 4; n <= 22; n++) {
    
    // 1. Criar um grafo aleatório não-orientado e não-pesado
    //    (0 = Digraph false, 0 = Weighted false)
    Graph* g = GraphCreate(n, 0, 0);

    // Adicionar arestas aleatórias
    // Vamos criar uma densidade média de 50%
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        // 50% de probabilidade de haver aresta entre i e j
        if ((rand() % 100) < 50) {
          GraphAddEdge(g, i, j);
        }
      }
    }

    // 2. Medir o tempo para GraphComputeMinDominatingSet
    //    Usamos cpu_time() do módulo instrumentation.h
    double start_time = cpu_time();
    
    IndicesSet* mds = GraphComputeMinDominatingSet(g);
    
    double time_mds = cpu_time() - start_time;
    IndicesSetDestroy(&mds); // Limpar resultado

    // 3. Medir o tempo para GraphComputeMinWeightDominatingSet
    start_time = cpu_time();
    
    IndicesSet* mwds = GraphComputeMinWeightDominatingSet(g);
    
    double time_mwds = cpu_time() - start_time;
    IndicesSetDestroy(&mwds); // Limpar resultado

    // 4. Imprimir os resultados
    //    O fflush garante que vemos o output linha a linha mesmo se o programa demorar
    printf("%d\t%f\t%f\n", n, time_mds, time_mwds);
    fflush(stdout);

    // Destruir o grafo antes da próxima iteração
    GraphDestroy(&g);
  }

  return 0;
}
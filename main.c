/*
 ============================================================================
 Name        : grafos.c
 Author      :
 Version     :
 Copyright   : Renan Augusto Starke, todos os direitos reservados
 Description : grafos com lista encadeadas, Ansi-style
             : estruturas disponiveis:
             : pilha e fila
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "grafo/grafo.h"

int compara_arestas(const void* a1, const void* a2) {
  int p1, p2;
  p1 = aresta_get_peso(((arestas_t*)a1));
  p2 = aresta_get_peso(((arestas_t*)a2));
  if (p1 < p2)
    return -1;
  else {
    if (p1 == p2)
      return 0;
    else
      return 1;
  }
}

int main(void) {

	grafo_t *grafo;
	vertice_t **vertice = malloc(sizeof(void*)*9);

	grafo = cria_grafo(0);

	//Adiciona todos os vertices no grafo
  // TODO: verificar se add vertice nao esta repetido

  for (int i = 0; i < 9; i++) {
    vertice[i] = grafo_adicionar_vertice(grafo, i);
  }
  adiciona_adjacentes(grafo, vertice[0], 4, 1, 4, 7, 8);
  adiciona_adjacentes(grafo, vertice[1], 4, 2, 8, 7, 11);
  adiciona_adjacentes(grafo, vertice[2], 6, 3, 7, 5, 4, 8, 2);
  adiciona_adjacentes(grafo, vertice[3], 2, 4, 9, 5, 14);
  adiciona_adjacentes(grafo, vertice[4], 2, 5, 10);
  adiciona_adjacentes(grafo, vertice[5], 2, 6, 2);
  adiciona_adjacentes(grafo, vertice[6], 4, 7, 1, 8, 6);
  adiciona_adjacentes(grafo, vertice[7], 2, 8, 7);
  adiciona_adjacentes(grafo, vertice[8], 6, 6, 14, 2, 7, 3, 9);

  int *arestas_tamanho = (int*)malloc(sizeof(int));
  arestas_t **arestas_arr = grafo_get_arestas_arr(grafo, arestas_tamanho);

  for (int i = 0; i < *arestas_tamanho; i++)
    printf("\npeso aresta: %d", aresta_get_peso((arestas_t*)arestas_arr[i]));

  //  qsort(arestas, **arestas_tamanho, aresta_tamanho_struct(), compara_arestas);

	exportar_grafo_dot("grafo.dot", grafo);
	//libera_grafo(grafo);

	return EXIT_SUCCESS;
}

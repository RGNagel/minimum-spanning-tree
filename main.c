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
  p1 = aresta_get_peso(*(arestas_t**)a1);
  p2 = aresta_get_peso(*(arestas_t**)a2);
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
	vertice_t **vertice = malloc(sizeof(vertice_t*)*9);

	grafo = cria_grafo(0);

	//Adiciona todos os vertices no grafo
  // TODO: verificar se add vertice nao esta repetido

  for (int i = 0; i < 9; i++) {
    vertice[i] =  grafo_adicionar_vertice(grafo, i);
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

  exportar_grafo_dot("grafo.dot", grafo);

  int *arestas_tamanho = (int*)malloc(sizeof(int));
  arestas_t **arestas_arr = grafo_get_arestas_arr(grafo, arestas_tamanho);

  #ifdef DEBUG
  for (int i = 0; i < *arestas_tamanho; i++)
    printf("\npeso aresta: %d", aresta_get_peso((arestas_t*)arestas_arr[i]));
  #endif

  qsort(arestas_arr, *arestas_tamanho, sizeof(arestas_t*), compara_arestas);

  #ifdef DEBUG
  printf("\nsorted:");
  for (int i = 0; i < *arestas_tamanho; i++)
    printf("\npeso aresta: %d", aresta_get_peso((arestas_t*)arestas_arr[i]));
  #endif

  grafo_t *grafo_mst = cria_grafo(1);

  for (int i = 0; i < *arestas_tamanho; i++) {
    // we must create new vertices for not cluttering original graph
    vertice_t *src, *dest;
    int id_src, id_dest;

    id_src = vertice_get_id(aresta_get_fonte(arestas_arr[i]));
    id_dest = vertice_get_id(aresta_get_dest(arestas_arr[i]));

    /* check if vertices were not added before. TODO: workaround for not taking O(N) */
    if (!procura_vertice(grafo_mst, id_src))
      src = grafo_adicionar_vertice(grafo_mst, id_src);
    if (!procura_vertice(grafo_mst, id_dest))
      dest = grafo_adicionar_vertice(grafo_mst, id_dest);

    // add ONE edge to both vertices
    adiciona_adjacentes(grafo_mst, src, 2, vertice_get_id(dest), aresta_get_peso(arestas_arr[i]));
    // when cycle happens, both two vertices are already in the graph so we do not remove them
    if (has_cycle(grafo_mst, lista_tamanho(grafo_get_vertices(grafo))     /* second arg. makes it possible to use array indices to find parent inside has_cycle */ ))
      // remove only edge, not vertices
      grafo_remove_ultima_aresta(grafo_mst);

    // check if edges = v - 1, if so then finish
    /*if (lista_tamanho(grafo_get_vertices(grafo_mst)) - 1 == lista_tamanho(grafo_get_arestas(grafo_mst))) {}
      break;*/
  }

  exportar_grafo_dot("grafo_mst.dot", grafo_mst);

  libera_grafo(grafo);
  free(vertice);
  free(arestas_tamanho);
  free(arestas_arr);

  libera_grafo(grafo_mst);

	return EXIT_SUCCESS;
}

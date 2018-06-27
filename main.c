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

#define DEBUG

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

	/* vertice_t **vertice = malloc(sizeof(vertice_t*)*9); */
  
  grafo = create_grafo_from_file("grafo_input.csv");

  /*

  grafo = cria_grafo(0);

	//Adiciona todos os vertices no grafo
  // TODO: verificar se add vertice nao esta repetido
  for (int i = 0; i < 9; i++)
    vertice[i] =  grafo_adicionar_vertice(grafo, i);

  adiciona_adjacentes(grafo, vertice[0], 4, 1, 4, 7, 8);
  adiciona_adjacentes(grafo, vertice[1], 4, 2, 8, 7, 11);
  adiciona_adjacentes(grafo, vertice[2], 6, 3, 7, 5, 4, 8, 2);
  adiciona_adjacentes(grafo, vertice[3], 4, 4, 9, 5, 14);
  adiciona_adjacentes(grafo, vertice[4], 2, 5, 10);
  adiciona_adjacentes(grafo, vertice[5], 2, 6, 2);
  adiciona_adjacentes(grafo, vertice[6], 4, 7, 1, 8, 6);
  adiciona_adjacentes(grafo, vertice[7], 2, 8, 7);

  */

  exportar_grafo_dot("grafo.dot", grafo);

  int *arestas_tamanho = (int*)malloc(sizeof(int));
  arestas_t **arestas_arr = grafo_get_arestas_arr(grafo, arestas_tamanho);

#ifdef DEBUG
  printf("\npeso: src -> dest");
  for (int i = 0; i < *arestas_tamanho; i++)
    printf("\n%d: %d -> %d",
           aresta_get_peso((arestas_t*)arestas_arr[i]),
           vertice_get_id(aresta_get_fonte((arestas_t*)arestas_arr[i])),
           vertice_get_id(aresta_get_dest((arestas_t*)arestas_arr[i]))
    );
#endif

  qsort(arestas_arr, *arestas_tamanho, sizeof(arestas_t*), compara_arestas);

#ifdef DEBUG
  printf("\nsorted:");
  printf("\npeso: src -> dest");
  for (int i = 0; i < *arestas_tamanho; i++)
    printf("\n%d: %d -> %d",
           aresta_get_peso((arestas_t*)arestas_arr[i]),
           vertice_get_id(aresta_get_fonte((arestas_t*)arestas_arr[i])),
           vertice_get_id(aresta_get_dest((arestas_t*)arestas_arr[i]))
    );
#endif

  grafo_t *grafo_mst = cria_grafo(1);

  /* get number of vertices. the mst graph will have qty of vertices minus 1 */
  int qty_v = lista_tamanho(grafo_get_vertices(grafo));

  for (int i = 0; i < *arestas_tamanho; i++) {
    // we must create new vertices for not cluttering original graph
    vertice_t *src, *dest;
    int id_src, id_dest;

    src = aresta_get_fonte(arestas_arr[i]);
    id_src = vertice_get_id(src);
    src = get_vertice_added_in_mst(src);

    dest = aresta_get_dest(arestas_arr[i]);
    id_dest = vertice_get_id(dest);
    dest = get_vertice_added_in_mst(dest);

    /* check - in O(1) - if vertices were already added in MST graph */
    if (!src)
      src = grafo_adicionar_vertice(grafo_mst, id_src);
    if (!dest)
      dest = grafo_adicionar_vertice(grafo_mst, id_dest);

    // add ONE edge to both vertices
    adiciona_adjacentes(grafo_mst, src, 2, id_dest, aresta_get_peso(arestas_arr[i]));

    // when cycle happens, both two vertices are already in the graph so we do not remove them
    if (has_cycle(grafo_mst, lista_tamanho(grafo_get_vertices(grafo))     /* second arg. makes it possible to use array indices to find parent inside has_cycle */ ))
      grafo_remove_ultima_aresta(grafo_mst);   // remove only edge, not vertices

    // check if edges = v - 1, if so then finish
    if (qty_v - 1 == lista_tamanho(grafo_get_arestas(grafo_mst)))
      break;
  }

  exportar_grafo_dot("grafo_mst.dot", grafo_mst);

  libera_grafo(grafo);
  //ofree(vertice);
  free(arestas_tamanho);
  free(arestas_arr);

  libera_grafo(grafo_mst);

	return EXIT_SUCCESS;
}

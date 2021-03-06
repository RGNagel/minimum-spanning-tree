/*
 * grafo.c
 *
 *  Created on: Jul 5, 2016
 *      Author: Renan Augusto Starke
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for memset */
#include <stdarg.h>
#include <limits.h>
#include "grafo.h"
#include "vertice.h"
#include "../lista_enc/lista_enc.h"
#include "../lista_enc/no.h"

#define FALSE 0
#define TRUE 1

#define INFINITO INT_MAX

struct grafos {
	int id;
	lista_enc_t *vertices;
  lista_enc_t *arestas; /* for ordering */
};

grafo_t *cria_grafo(int id)
{
	grafo_t *p = NULL;

	p = (grafo_t*) malloc(sizeof(grafo_t));

	if (p == NULL)	{
		perror("cria_grafo:");
		exit(EXIT_FAILURE);
	}

	p->id = id;
	p->vertices = cria_lista_enc();
  p->arestas = cria_lista_enc();

	return p;
}

vertice_t* grafo_adicionar_vertice(grafo_t *grafo, int id)
{
	vertice_t *vertice;
	no_t *no;

#ifdef DEBUG
	printf("grafo_adicionar_vertice: %d\n", id);
#endif

	if (grafo == NULL)	{
			fprintf(stderr,"grafo_adicionar_vertice: grafo invalido!");
			exit(EXIT_FAILURE);
	}

	vertice = cria_vertice(id);
	no = cria_no(vertice);

	add_cauda(grafo->vertices, no);

	return vertice;
}

vertice_t* procura_vertice(grafo_t *grafo, int id)
{
	no_t *no_lista;
	vertice_t *vertice;
	int meu_id;

	if (grafo == NULL)	{
		fprintf(stderr,"procura_vertice: grafo invalido!");
		exit(EXIT_FAILURE);
	}

	if (lista_vazia(grafo->vertices) == TRUE)
		return FALSE;

	no_lista = obter_cabeca(grafo->vertices);

	while (no_lista)
	{
		//obtem o endereco da lista
		vertice = obter_dado(no_lista);

		//obterm o id do vertice
		meu_id = vertice_get_id(vertice);

		if (meu_id == id) {
			return vertice;
		}

		no_lista = obtem_proximo(no_lista);
	}

	return NULL;
}

void adiciona_adjacentes(grafo_t *grafo, vertice_t *vertice, int n, ...)
{
	va_list argumentos;
	vertice_t *sucessor;
	arestas_t *aresta;

	int id_sucessor;
	int peso;
        int x;

	/* Initializing arguments to store all values after num */
	va_start (argumentos, n);

	for (x = 0; x < n; x=x+2 )
	{
		id_sucessor = va_arg(argumentos, int);
		peso = va_arg(argumentos, int);

		sucessor = procura_vertice(grafo, id_sucessor);

		if (sucessor == NULL) {
			fprintf(stderr, "adiciona_adjacentes: sucessor nao encontrado no grafo\n");
			exit(EXIT_FAILURE);
		}
    
		aresta = cria_aresta(vertice, sucessor, peso);
    /* sucessor does not receive aresta in its struct */
		adiciona_aresta(grafo, vertice, aresta); /* inclui grafo como arg. p/ preencher a lista de arestas na struct do grafo*/

#ifdef DEBUG
		printf("\tvertice: %d\n", vertice_get_id(vertice));
		printf("\tsucessor: %d\n", id_sucessor	);
		printf("\tpeso: %d\n", peso);
#endif
	}

	va_end (argumentos);
}

void exportar_grafo_dot(const char *filename, grafo_t *grafo)
{
	FILE *file;

	no_t *no_vert;
	no_t *no_arest;
	vertice_t *vertice;
	vertice_t *adjacente;
	arestas_t *aresta;
	arestas_t *contra_aresta;
	lista_enc_t *lista_arestas;

	int peso;

	if (filename == NULL || grafo == NULL){
		fprintf(stderr, "exportar_grafp_dot: ponteiros invalidos\n");
		exit(EXIT_FAILURE);
	}

	file = fopen(filename, "w");

	if (file == NULL){
		perror("exportar_grafp_dot:");
		exit(EXIT_FAILURE);
	}

	fprintf(file, "graph {\n");

	//obtem todos os nos da lista
	no_vert = obter_cabeca(grafo->vertices);
	while (no_vert) {
		vertice = obter_dado(no_vert);

    //obtem todos as arestas
		lista_arestas = vertice_get_arestas(vertice);

    /*
    #ifdef DEBUG
    printf("\nvertice id in exported: %d", vertice_get_id(vertice));
    printf("\ntamanho arestas do vertice: %d", lista_tamanho(lista_arestas));
    #endif
    */

		no_arest = obter_cabeca(lista_arestas);
		while (no_arest) {
			aresta = obter_dado(no_arest);

			//ignora caso já exportada
			if (aresta_get_status(aresta) == EXPORTADA) {
				no_arest = obtem_proximo(no_arest);
				continue;
			}

			//marca como exportada esta aresta
			aresta_set_status(aresta, EXPORTADA);
			adjacente = aresta_get_adjacente(aresta);
			//marca contra-aresta também como exporta no caso de grafo não direcionados
			contra_aresta = procurar_adjacente(adjacente, vertice);
			if (contra_aresta != NULL)
				aresta_set_status(contra_aresta, EXPORTADA);
			//obtem peso
			peso = aresta_get_peso(aresta);
      
			fprintf(file, "\t%d -- %d [label = %d];\n",
					vertice_get_id(vertice),
					vertice_get_id(adjacente),
					peso);

			no_arest = obtem_proximo(no_arest);
		}
		no_vert = obtem_proximo(no_vert);
	}
	fprintf(file, "}\n");
	fclose(file);
}


void libera_grafo (grafo_t *grafo){
	no_t *no_vert;
	no_t *no_arest;
	no_t *no_liberado;
	vertice_t *vertice;
	arestas_t *aresta;
	lista_enc_t *lista_arestas;

	if (grafo == NULL) {
		fprintf(stderr, "libera_grafo: grafo invalido\n");
		exit(EXIT_FAILURE);
	}

	//varre todos os vertices
	no_vert = obter_cabeca(grafo->vertices);
	while (no_vert){
		vertice = obter_dado(no_vert);
    
		//libera todas as arestas
		lista_arestas = vertice_get_arestas(vertice);
		no_arest = obter_cabeca(lista_arestas);
		while (no_arest){
			aresta = obter_dado(no_arest);
      
			//libera aresta
			free(aresta);

			//libera no da lsita
			no_liberado = no_arest;
			no_arest = obtem_proximo(no_arest);
			free(no_liberado);
		}

		//libera lista de arestas e vertice
		free(lista_arestas);
		free(vertice);

		//libera no da lista
		no_liberado = no_vert;
		no_vert = obtem_proximo(no_vert);
		free(no_liberado);
	}
  // free nodes in grafo->arestas
  no_arest = obter_cabeca(grafo->arestas);
  while (no_arest) {
    // we do not need to free the data as it was already free in loop before
    no_liberado = no_arest;
    no_arest = obtem_proximo(no_arest);
    free(no_liberado);
  }
	//libera grafo e vertice
	free(grafo->vertices);
  free(grafo->arestas);
	free(grafo);
}

static int _find_parent(int *parent, int v)
{
  return (-1 == parent[v]) ? v : _find_parent(parent, parent[v]);
}

static void _union(int *parent, int set1, int set2)
{
  set1 = _find_parent(parent, set1);
  set2 = _find_parent(parent, set2);
  parent[set1] = set2; /* or vice-versa */
}

int has_cycle(grafo_t *grafo, int n_vertices)
{
  int *parent = (int*) malloc(sizeof(int)*n_vertices); /* temp vector */
  memset(parent, -1, sizeof(int)*n_vertices); /* init all elements with -1 (no parent) */

  no_t *no_temp = obter_cabeca(grafo->arestas);
  while (no_temp) {

    arestas_t *aresta_temp = obter_dado(no_temp);
    vertice_t *v1 = aresta_get_fonte(aresta_temp);
    vertice_t *v2 = aresta_get_dest(aresta_temp);

    int v1_p = _find_parent(parent, vertice_get_id(v1));
    int v2_p = _find_parent(parent, vertice_get_id(v2));

    if (v1_p == v2_p) {
      free(parent);
      return 1; /* has cycle */
    }

    _union(parent, v1_p, v2_p);
    no_temp = obtem_proximo(no_temp);
  }
  free(parent);
  return 0; /* no cycle */
}

arestas_t **grafo_get_arestas_arr(grafo_t *grafo, int *tamanho_arr /* tamanho p/ preencher */)
{

  if (grafo == NULL) {
    fprintf(stderr, "grafo_get_arestas_arr: invalid data");
		exit(EXIT_FAILURE);
  }

  lista_enc_t *arestas = grafo_get_arestas(grafo);
  *tamanho_arr = lista_tamanho(arestas);

  arestas_t **arestas_arr = (arestas_t**) malloc(sizeof(void*)*(*tamanho_arr));

  no_t *no_temp = obter_cabeca(arestas);
  arestas_t **arestas_arr_temp = arestas_arr; /* ptr temp p/ incrementar sem perder referencia de posicao do array original */
  while (no_temp != NULL) {
    *arestas_arr_temp = (arestas_t*)obter_dado(no_temp);
    no_temp = obtem_proximo(no_temp);
    *arestas_arr_temp++;
  }
  return arestas_arr;
}

void adiciona_aresta(grafo_t *grafo, vertice_t *vertice, arestas_t *aresta)
{
	no_t *no;

	if (vertice == NULL || aresta == NULL || grafo == NULL)	{
		fprintf(stderr, "adiciona_aresta: dados invalidos\n");
		exit(EXIT_FAILURE);
	}

	no = cria_no(aresta);
	add_cauda(vertice_get_arestas(vertice), no);
  no = cria_no(aresta); /* we need to create NEW node for any OTHER list. Else, node from first list will point to next node in second list */
  add_cauda(grafo->arestas, no);

}

lista_enc_t *grafo_get_arestas(grafo_t *grafo)
{
  if (grafo == NULL) {
    fprintf(stderr, "grafo_get_arestas: vertice invalido\n");
    exit(EXIT_FAILURE);
  }
  return grafo->arestas;
}

lista_enc_t *grafo_get_vertices(grafo_t *grafo)
{
  if (grafo == NULL) {
    fprintf(stderr, "grafo_get_vertices: vertice invalido\n");
    exit(EXIT_FAILURE);
  }
  return grafo->vertices;
}

void grafo_remove_ultima_aresta(grafo_t *grafo)
{
  no_t *no = remover_cauda(grafo->arestas);
  arestas_t *aresta = obter_dado(no);
  free(no); // free node to edge

  // free NODES to edge
  vertice_t *src = aresta_get_fonte(aresta);
  no = remover_cauda(vertice_get_arestas(src));
  free(no);
  // do not remove aresta from dest
  // free DATA only once
  free(aresta);
}

grafo_t* create_grafo_from_file(char *filename) {
  int lines = 0, i = 0;
  int src, dest, weight;

  grafo_t *grafo = cria_grafo(0);

  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "unvalid file opening\n");
    exit(EXIT_FAILURE);
  }

  /* read amount of lines; avoiding malloc each time for each line of file */

  fscanf(file, "%*[^\n]"); /* jump header line */
  while (fscanf(file, "%d,%d,%d\n", &src, &dest, &weight) != EOF)
    lines++;

  printf("%i", lines);
  fflush(stdout);

  vertice_t **vertice = malloc(sizeof(vertice_t*)*lines);
  fseek(file, 0, SEEK_SET);

  for (i = 0; i < lines; i++)
    vertice[i] = grafo_adicionar_vertice(grafo, i);

  i = 0;
  fscanf(file, "%*[^\n]"); /* jump header line */
  while (fscanf(file, "%d,%d,%d\n", &src, &dest, &weight) != EOF)
    adiciona_adjacentes(grafo, vertice[src], 2, dest, weight);

  fclose(file);

  return grafo;
}

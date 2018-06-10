objects = main.o grafo/grafo.o grafo/vertice.o lista_enc/no.o lista_enc/lista_enc.o
output_file = program.out


$(output_file) : $(objects)
	cc -o $(output_file) $(objects)

main.o: lista_enc/lista_enc.h grafo/grafo.h grafo/vertice.h
grafo/grafo.o: grafo/grafo.h lista_enc/lista_enc.h grafo/vertice.h
grafo/vertice.o: grafo/vertice.h lista_enc/lista_enc.h
lista_enc/lista_enc.o: lista_enc/lista_enc.h lista_enc/no.h
lista_enc/no.o: lista_enc/no.h

clean:
	rm -f *.o grafo/*.o lista_enc/*.o pilha/*.o fila/*.o binaria/*.o *.dot *.svg *.out

hello_world:
	echo "Hello World"

install:
	sudo apt-get install graphviz imagemagick

show: $(output_file)
	dot -Tsvg *.dot | display


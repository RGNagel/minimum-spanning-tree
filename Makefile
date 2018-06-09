objects = main.o pilha/pilha.o fila/fila.o lista_enc/no.o lista_enc/lista_enc.o binaria/tree.o heap/heap.o
output_file = ex.out


$(output_file) : $(objects)
	cc -o $(output_file) $(objects)

main.o: lista_enc/lista_enc.h binaria/tree.h heap/heap.h
pilha/pilha.o: lista_enc/lista_enc.h lista_enc/no.h pilha/pilha.h
fila/fila.o: lista_enc/lista_enc.h lista_enc/no.h fila/fila.h
lista_enc/lista_enc.o: lista_enc/lista_enc.h lista_enc/no.h
lista_enc/no.o: lista_enc/no.h
binaria/tree.o: binaria/tree.h lista_enc/lista_enc.h
heap/heap.o: heap/heap.h

clean:
	rm -f *.o lista_enc/*.o pilha/*.o fila/*.o binaria/*.o *.dot *.svg *.out

hello_world:
	echo "Hello World"

install:
	sudo apt-get install graphviz imagemagick

show: $(output_file)
	dot -Tsvg *.dot | display


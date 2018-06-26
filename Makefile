objects = main.o grafo/grafo.o grafo/vertice.o lista_enc/no.o lista_enc/lista_enc.o
output_file = program.out


$(output_file) : $(objects)
	cc -o $(output_file) $(objects)

main.o: lista_enc/lista_enc.h grafo/grafo.h grafo/vertice.h
grafo/grafo.o: grafo/grafo.h lista_enc/lista_enc.h grafo/vertice.h
grafo/vertice.o: grafo/vertice.h lista_enc/lista_enc.h
lista_enc/lista_enc.o: lista_enc/lista_enc.h lista_enc/no.h
lista_enc/no.o: lista_enc/no.h

.PHONY: clean
clean:
	rm -f *.o grafo/*.o lista_enc/*.o pilha/*.o fila/*.o binaria/*.o *.dot *.svg *.out *.png

.PHONY: hello_world
hello_world:
	echo "Hello World"

.PHONY: install
install:
	sudo apt-get install graphviz imagemagick

output_image = grafos.png

image: $(output_file)
	# for each .dot file -> convert to .dot.png file
	# join *.dot.png files into final file
	# remove *.dot.png files (intermediaries)
	find . -iname "*.dot" -exec bash -c 'dot -Tpng {} > {}.png' \; && \
	convert +append *.dot.png $(output_image) && \
	rm *.dot.png

show: $(output_file)
	display $(output_image)

run: $(output_file)
	./$(output_file)

.PHONY: all_steps
all_steps:
	$(MAKE) \
	$(MAKE) run \
	$(MAKE) image \
	$(MAKE) show \
	$(MAKE) clean

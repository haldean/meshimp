all:
	make -C src
	cp src/meshimp .

clean:
	make -C src clean

run: all
	./meshimp

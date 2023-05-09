.DEFAULT_GOAL := default

GCC=/usr/bin/gcc

default: shell.o
	$(GCC) -Wall main.c shell.o -o lsh -g

shell.o: shell.c shell.h
	$(GCC) -Wall shell.c -c -o shell.o -g

clean:
	rm -rf *.o lsh lsh.*

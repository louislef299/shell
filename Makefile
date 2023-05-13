.DEFAULT_GOAL := default

GCC=/usr/bin/gcc

default: shell.o disk.o
	$(GCC) -Wall main.c shell.o disk.o -o lsh -g

shell.o: shell.c shell.h
	$(GCC) -Wall shell.c -c -o shell.o -g

disk.o: disk.c disk.h
	$(GCC) -Wall disk.c -c -o disk.o -g

clean:
	rm -rf *.o lsh lsh.*

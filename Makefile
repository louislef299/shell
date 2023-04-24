.DEFAULT_GOAL := default

GCC=/usr/bin/gcc
FLAGS=-Wall -g


default: 
	$(GCC) $(FLAGS) main.c -o lsh

clean:
	rm -rf *.o lsh lsh.*

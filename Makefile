.DEFAULT_GOAL := default

GCC ?= /usr/bin/gcc

default: shell.o fs.o disk.o
	$(GCC) -Wall main.c shell.o disk.o fs.o -o lsh -g

shell.o: shell.c shell.h
	$(GCC) -Wall shell.c -c -o shell.o -g

fs.o: fs.c fs.h
	$(GCC) -Wall fs.c -c -o fs.o -g

disk.o: disk.c disk.h
	$(GCC) -Wall disk.c -c -o disk.o -g

container:
	docker build -t lsh .

clean:
	rm -rf *.o lsh lsh.*
	docker container prune -f && docker image prune -f

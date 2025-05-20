.DEFAULT_GOAL := default

# https://andrewkelley.me/post/zig-cc-powerful-drop-in-replacement-gcc-clang.html
GCC ?= zig cc

default: src/shell.o src/fs.o src/disk.o
	$(GCC) -Wall src/main.c src/shell.o src/disk.o src/fs.o -o lsh -g

src/shell.o: src/shell.c src/shell.h src/fs.h
	$(GCC) -Wall src/shell.c -c -o src/shell.o -g

src/fs.o: src/fs.c src/fs.h src/disk.h
	$(GCC) -Wall src/fs.c -c -o src/fs.o -g

src/disk.o: src/disk.c src/disk.h
	$(GCC) -Wall src/disk.c -c -o src/disk.o -g

container:
	docker build -t lsh .

clean:
	rm -rf src/*.o lsh lsh.*
	docker container prune -f && docker image prune -f
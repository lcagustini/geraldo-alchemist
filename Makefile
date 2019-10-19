all: src/main.c src/data.c src/data.h
	mkdir -p bin
	gcc src/main.c -o bin/main.x -lraylib

run: all
	./bin/main.x

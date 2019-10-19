all: src/main.c src/data.c src/data.h
	mkdir -p bin
	gcc -Wall src/main.c -o bin/main.x -lraylib -lm

run: all
	./bin/main.x

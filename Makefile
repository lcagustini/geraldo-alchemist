all: src/main.c
	mkdir -p bin
	gcc src/main.c -o bin/main.x -lraylib

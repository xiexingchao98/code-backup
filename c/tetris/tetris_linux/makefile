OBJ=./obj
BIN=./bin

compile: $(subst .c,.o,$(wildcard *.c))

cmptetris: tetris.c kbhit.c
	gcc -g tetris.c kbhit.c -o $(BIN)/tetris
runtetris:
	./bin/tetris

%.o:%.c
	gcc $< -o $@

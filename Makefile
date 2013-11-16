all: clean build run

build: 
	gcc -Wall -ansi -std=c99 -O3 -lncurses -o menu status_menu.c menu.c 

clean:
	rm -f *.o menu

run: menu
	./menu

set: clean build run

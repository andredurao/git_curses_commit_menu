all: clean build run

build: 
	gcc -Wall -ansi -std=c99 -O3 -lz -lncurses -lgit2 -o menu status_menu.c git_status.c menu.c 

clean:
	rm -f *.o menu

run: menu
	./menu

set: clean build run

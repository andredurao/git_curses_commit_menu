all: clean build run

build: 
        #OSX
#	gcc -Wall -ansi -std=c99 -O3 -lz -lncursesw -lgit2 -o menu status_menu.c git_status.c menu.c -I/usr/include/ncursesw
        #LINUX : gcc -o diff diff.c -lgit2
	gcc -o menu git_status.c status_menu.c menu.c -lncursesw -lgit2
clean:
	rm -f *.o menu

run: menu
	./menu

set: clean build run

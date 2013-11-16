.PHONY: menu

menu: clean build

build: menu.c
	gcc -o menu menu.c -lncurses

clean:
	rm -f *.o menu


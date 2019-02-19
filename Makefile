make: test.c
	chmod 644 test.c
	gcc -Wall -std=c11 test.c -lmenu -lncurses

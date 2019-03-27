make: test3.c
	chmod 644 main.c
	chmod 644 movie_records
	gcc -g -std=c99 main.c -lncurses

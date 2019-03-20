make: test3.c
	chmod 644 test3.c
	chmod 644 datatest2.txt
	chmod 644 movie_records
	gcc -g -std=c99 test3.c -lncurses

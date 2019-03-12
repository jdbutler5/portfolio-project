make: test3.c
	chmod 644 test3.c
	chmod 644 datatest2.txt
	chmod 644 movie_records
	gcc -g -std=c11 test3.c -lmenu -lncurses

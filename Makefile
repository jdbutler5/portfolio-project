make: main.c
	chmod 644 main.c
	chmod 644 movie_records
	gcc -std=c99 main.c -o imdb_database -lncurses

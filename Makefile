make: main.c
	chmod 644 main.c
	chmod 644 movie_records
	gcc -std=c11 main.c -o imdb_database -lncurses

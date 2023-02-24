CC = gcc
FLAGS = -g -std=c99 -pedantic -W -Werror
FILE = main.c bin_search.c qsort.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

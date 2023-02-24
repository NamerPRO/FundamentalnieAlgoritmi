CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c heap_priority_queue.c cstring.c coverflow.c cnumber.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

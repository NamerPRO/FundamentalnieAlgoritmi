CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c clist.c cstring.c coverflow.c cnumber.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

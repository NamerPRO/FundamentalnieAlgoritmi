CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c cbst.c clist.c cstring.c ctrie.c coverflow.c cnumber.c cio.c statistics.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

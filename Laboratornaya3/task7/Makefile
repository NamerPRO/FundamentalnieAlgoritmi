CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c clist.c villager.c date.c cstring.c cfile.c cnumber.c coverflow.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

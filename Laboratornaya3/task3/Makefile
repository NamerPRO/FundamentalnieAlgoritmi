CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c clist.c cfibo.c cstring.c cfile.c carray.c cnumber.c coverflow.c employee.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

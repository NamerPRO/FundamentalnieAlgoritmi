CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c cstring.c student.c cio.c cinteger.c coverflow.c clist.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

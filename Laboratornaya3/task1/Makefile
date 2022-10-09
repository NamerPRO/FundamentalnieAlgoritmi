CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c coverflow.c cinteger.c cstring.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

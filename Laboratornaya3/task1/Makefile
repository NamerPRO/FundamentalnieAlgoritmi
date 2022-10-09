CC = gcc
FLAGS = -g -std=c99 -pedantic -W -Werror
FILE = main.c coverflow.c cinteger.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

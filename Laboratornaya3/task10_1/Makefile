CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c tree_builder.c cstring.c cstack.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

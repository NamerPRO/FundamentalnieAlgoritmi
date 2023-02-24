CC = gcc
FLAGS = -g -std=c99 -pedantic -W -Werror -lm
FILE = main.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

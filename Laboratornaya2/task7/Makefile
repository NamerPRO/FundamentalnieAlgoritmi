CC = gcc
FLAGS = -g -std=c99 -pedantic -W -Werror -lm
FILE = main.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

run2: main2.c
	$(CC) main2.c -o $@.out $(FLAGS)

clear:
	rm -f *.out

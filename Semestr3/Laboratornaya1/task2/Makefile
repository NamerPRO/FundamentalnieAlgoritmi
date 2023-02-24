CC = gcc
FLAGS = -g -std=c99 -pedantic -Wextra -w -pipe -Werror -lm
FILE = main.c overflow.c

run: $(FILE)
	$(CC) $(FILE) -o $@ $(FLAGS)

clear:
	rm -f run

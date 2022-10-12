CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c cstring.c cmessage.c cio.c cinteger.c coverflow.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

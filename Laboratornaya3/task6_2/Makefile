CC = gcc
FLAGS = -std=c99 -pedantic -g -W -Werror
FILE = main.c cstring.c mail.c cio.c coverflow.c cinteger.c carray.c date.c

run: $(FILE)
	$(CC) $(FILE) -o $@.out $(FLAGS)

clear:
	rm -f *.out

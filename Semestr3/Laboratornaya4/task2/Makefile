CC = gcc
FLAGS = -std=c99 -pedantic -Wextra -w -pipe -Werror -pthread -fcommon
CLIENT_FILES = hash_table_polynomial.c csemophore.c cpolymath.c cmonomial.c cpolynomial.c cpair.c hash_table.c clist.c client.c cserializer.c cstring.c coverflow.c cnumber.c
SERVER_FILES = hash_table_polynomial.c csemophore.c cpolymath.c cmonomial.c cpolynomial.c cpair.c hash_table.c clist.c server.c cserializer.c cstring.c coverflow.c cnumber.c
DEBUG = -g -D___DEBUG___ -D___DEBUG_CLIENT___
OS = ___UNIX___ #___WINDOWS___
MATH = -lm

run: client server

client: $(CLIENT_FILES)
	$(CC) $(FLAGS) $(CLIENT_FILES) -o $@ $(DEBUG) -D$(OS) $(MATH)

server: $(SERVER_FILES)
	$(CC) $(FLAGS) $(SERVER_FILES) -o $@ $(DEBUG) -D$(OS) $(MATH)

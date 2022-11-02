#ifndef ___CFILE___
#define ___CFILE___

#include <stdio.h>
#include <ctype.h>

#include "cstring.h"
#include "cconstants.h"

int standart_string_end(char symbol);
int file_read_until(FILE* file, string** str_str, int (*cmp)(char symbol), int string_type);
int file_read_n_until(FILE* file, string** str_str, int count, int (*cmp)(char symbol), int string_type);

#endif

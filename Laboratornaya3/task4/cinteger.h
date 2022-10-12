#ifndef ___CINTEGER___
#define ___CINTEGER___

#include <string.h>

#include "cstring.h"
#include "coverflow.h"

#define NAN 2
#define NUMBER 3

int get_digit_from_string(string* digit_str, int* execute_status);
int is_symbol_digit(char symbol);

#endif

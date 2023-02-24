#ifndef ___CNUMBER___
#define ___CNUMBER___

#include <string.h>

#include "cstring.h"
#include "coverflow.h"

#define NAN 2
#define NUMBER 3

int get_integer_from_string(string* digit_str, int* execute_status);
double get_double_from_string(string* digit_str, int* execute_status);
int is_symbol_digit(char symbol);

#endif

#ifndef ___DECIMAL___
#define ___DECIMAL___

#include <string.h>
#include <math.h>
#include <ctype.h>

#include "overflow.h"

int safe_to_dec(char* num, int sys, int* is_overflowed);
int get_decimal_value(char symbol);

#endif

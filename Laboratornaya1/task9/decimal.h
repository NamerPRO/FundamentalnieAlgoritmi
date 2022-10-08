#ifndef ___DECIMAL___
#define ___DECIMAL___

#include <string.h>
#include <math.h>
#include <ctype.h>

#include "overflow.h"

int safe_to_dec(char* num, int sys, int* is_overflowed);
char* dec_to_system(int number, int system, char* result_number);
int is_number_in_required_system(char* number, int system);
int get_decimal_value(char symbol);

#endif

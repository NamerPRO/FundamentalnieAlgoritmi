#ifndef ___CEXPRESSION___
#define ___CEXPRESSION___

#include <math.h>

#include "cstring.h"
#include "stack.h"
#include "stack_int.h"
#include "cconstants.h"
#include "cnumber.h"

int validate_brackets(string* expression);
int is_operator(char arg);
int operand_cmp(int symbol, int index);
string* get_postfix_form(string* expression, int* execute_status);
int get_priority(int symbol);

#endif

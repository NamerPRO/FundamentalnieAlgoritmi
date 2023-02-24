#ifndef ___COMMANDS_API___
#define ___COMMANDS_API___

#include <stdlib.h>

#include "cnumber.h"
#include "cstring.h"

int command_next(int symbol, int index);
int get_next_argument(string* argument, string* arguments, unsigned long int* start);

#endif

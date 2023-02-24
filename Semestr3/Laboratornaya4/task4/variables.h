#ifndef ___VARIABLES___
#define ___VARIABLES___

#include <stdlib.h>

#include "cstring.h"

void get_variable_or_null(char name, int** array, int* execute_status);
int update_variable(char name, int* variable, int size);
int get_array_size(char name, int* execute_status);
void kill_variables();

#endif

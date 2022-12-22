#ifndef ___STANDART_COMMANDS_LIB___
#define ___STANDART_COMMANDS_LIB___

#include <stdlib.h>

#include "cstring.h"
#include "commands_api.h"

void load(string* arguments, unsigned long int start);
void save(string* arguments, unsigned long int start);
void _rand(string* arguments, unsigned long int start);
void concat(string* arguments, unsigned long int start);
void _free(string* arguments, unsigned long int start);
void _remove(string* arguments, unsigned long int start);
void copy(string* arguments, unsigned long int start);
void sort(string* arguments, unsigned long int start);
void shuffle(string* arguments, unsigned long int start);
void stats(string* arguments, unsigned long int start);
void print(string* arguments, unsigned long int start);

int compare_int_max(const void* lhs, const void* rhs);
int compare_int_min(const void* lhs, const void* rhs);

#endif

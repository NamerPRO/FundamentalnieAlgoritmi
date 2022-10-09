#ifndef ___CSTRING___
#define ___CSTRING___

#include <stdio.h>
#include <stdlib.h>

#define STANDART_STRING_SIZE 255
#define MEMORY_ALLOCATE_EXCEPTION 5
#define SUCCESS_FUNCTION_RETURN 6

typedef char* string;

int create_string(string* str);
int read_string(string* str, int (*cmp)(int x));
void free_string(string str);

#endif

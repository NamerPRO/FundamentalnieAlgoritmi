#ifndef ___CSTRING___
#define ___CSTRING___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define STANDART_STRING_SIZE 255
#define STANDART_ARRAY_SIZE 25
#define MEMORY_ALLOCATE_EXCEPTION 5
#define SUCCESS_FUNCTION_RETURN 6

typedef struct string {
  char* str;
  unsigned long int size;
} string;


void create_string(string* str, char* value);
int create_empty_string(string* str);
int read_string(FILE* stream, string* str, int (*cmp)(int x));
unsigned long int get_string_size(string* str);
void print_string(FILE* stream, string* str);
void free_string(string* str);
char* get_raw_string(string* str);
int standart_string_comporator(string* str1, string* str2);
int string_compare(string* str1, string* str2, int (*cmp)(string* str1, string* str2));
int create_string_duplicate(string* str, string* cpy_str);
int string_concat(unsigned long int count, string* out_str, ...);
int string_copy(string* from, string* to);
char* remove_data_from_string(string* str, unsigned long int left_bound, unsigned long int right_bound);


#endif

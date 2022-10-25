#ifndef ___CSTRING___
#define ___CSTRING___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>

#include "cconstants.h"

typedef struct string {
  char* str;
  unsigned long int size;
} string;

int create_string(string* str, char* value);
int create_all_strings(unsigned long int count, ...);
int create_empty_string(string* str);
int create_all_empty_strings(unsigned long int count, ...);
int read_string(FILE* stream, string* str, int (*cmp)(int x));
int read_all_strings(FILE* stream, int (*cmp)(int x), unsigned long int count, ...);
unsigned long int get_string_size(string* str);
void print_string(FILE* stream, char* pattern, ...);
void free_string(string* str);
void free_all_strings(unsigned long int count, ...);
char* get_raw_string(string* str);
int standart_string_comporator(string* str1, string* str2);
int string_compare(char* pattern, int (*cmp)(string* str1, string* str2), string* str1, ...);
int create_string_duplicate(string* str, string* cpy_str);
int string_concat(char* pattern, string* out_str, ...);
int string_copy(string* from, string* to);
//Добавить string_cut, вырезающий часть строки
//Добавить string_trim, что сжимает строку до ее размера
void string_gnaw(string* str, unsigned long int left_bound, unsigned long int right_bound);
char* str(char* value, int* execute_status);
int reset_string(string* str);
char* get_trimmed_string(string* str, int* execute_status);
char standart_random_character(unsigned long int position);
int get_random_string(string* str, unsigned long int size, char (*rnd_char)(unsigned long int position));
int string_contains_only(string* str, int (*in_range)(char symbol));
int standart_string_range(char symbol);
int standart_alphabet_comporator(string* str1, string* str2);
void to_upper_case(string* str);
void to_lower_case(string* str);
int not(int status_code);
int to_string(string* str_str, char* pattern, ...);
int string_assign(string* str_str, char* str_new);
void string_reverse(string* str_str, unsigned long int lhs, unsigned long int rhs);
int string_is_integer(string* str_str);
int standart_numeric_range(char symbol);

#endif

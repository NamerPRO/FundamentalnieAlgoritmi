#ifndef ___CSTRING___
#define ___CSTRING___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>

#include "cconstants.h"

#define EMPTY NULL
#define STACK 0
#define HEAP 1

#define LL 0
#define LG 1
#define GL 2
#define GG 3

#define FROM_STRING NULL

//For read_string
#define INCLUDE_CHARACTER 0
#define SKIP_CHARACTER -1
#define STOP_ITERATION 1
#define STOP_AND_INCLUDE 2
//===

typedef struct string {
  char* str;
  unsigned long int size;
} string;

typedef struct sized_string {
  char* str;
  unsigned long int size;
  unsigned long int data_size;
} sized_string;

//shrink_to_size
int create_string(string* str, char* value);
int create_sized_string(sized_string* str_str, char* value);
int create_all_strings(unsigned long int count, ...);
int create_empty_string(string* str);
int create_empty_sized_string(sized_string* str_str);
int create_all_empty_strings(unsigned long int count, ...);
// int read_string(FILE* stream, string* str, int (*cmp)(int x));
int read_string(FILE* stream, string* str, int (*cmp)(int symbol, int index), ...);
int read_all_strings(FILE* stream, int (*cmp)(int x), unsigned long int count, ...);
int read_arr_strings(FILE* stream, int (*cmp)(int x), unsigned long int count, string** str_arr);
unsigned long int get_string_size(string* str);
unsigned long int get_sized_string_size(sized_string* str);
void print_string(FILE* stream, char* pattern, ...);
void free_string(string* str);
void free_heap_string(string* str);
void free_heap_sized_string(sized_string* str);
void free_all_strings(unsigned long int count, ...);
char* get_raw_string(string* str);
char* get_raw_sized_string(sized_string* str);
int standart_string_comporator(string* str1, string* str2);
int string_compare(char* pattern, int (*cmp)(string* str1, string* str2), int* execute_status, string* str1, ...);
int create_string_duplicate(string* str, string* cpy_str);
int string_concat(char* pattern, string* out_str, ...);
int sized_string_concat(char* pattern, sized_string* out_str, ...);
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
int string_is_double(string* str_str);
int standart_numeric_range(char symbol);
int create_string_array(string*** str_arr, unsigned long int size);
void free_string_array(string*** str_arr, int size);
string* heap_string(char* c_str);
sized_string* heap_sized_string(char* c_str);
int read_string_to(string* str_str, unsigned long int* start, string* out_str, int (*cmp)(int symbol));
int scompare(string* lhs, char* rhs, int* execute_status);
int scompare_s(string* lhs, string* rhs);
int string_multy_compare(char* pattern, int (*cmp)(string* str1, string* str2), int* execute_status, string* str1, ...);
char sat(string* str, unsigned long int index);
char _char(string* str);
char __char(sized_string* str);
int cin(char* symbol, int from, int to, int flag);
int is_string_empty(string* str);
int is_sized_string_empty(sized_string* str);
int isspace_s(int symbol, int index);
string* clear_size(sized_string* sized_str);
string* heap_string_from_char(char symbol);
int sized_string_reset(sized_string* sstr);

#endif

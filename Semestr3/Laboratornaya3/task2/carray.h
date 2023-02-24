#ifndef ___CARRAY___
#define ___CARRAY___

#include <stdlib.h>

#include "cconstants.h"

#define STANDART_ARRAY_SIZE 10

typedef struct int_array {
  int* array;
  unsigned long int total_size;
  unsigned long int real_size;
} int_array;

int* get_int_array(int_array* array);
unsigned long int get_int_array_size(int_array* array);
int create_int_array(int_array* array);
int insert_int_in_array(int_array* array, int element);
void free_int_array(int_array* array);

#endif

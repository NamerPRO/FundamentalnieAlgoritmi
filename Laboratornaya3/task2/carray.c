#include "carray.h"

int* get_int_array(int_array* array) {
  return array->array;
}

unsigned long int get_int_array_size(int_array* array) {
  return array->real_size;
}

int create_int_array(int_array* array) {
  array->array = (int*)malloc(sizeof(int) * STANDART_ARRAY_SIZE);
  if (array->array == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  array->total_size = STANDART_ARRAY_SIZE;
  array->real_size = 0;
  return SUCCESS_FUNCTION_RETURN;
}

int insert_int_in_array(int_array* array, int element) {
  if (array->real_size == array->total_size) {
    int* possible_array = (int*)realloc(array->array, 2 * sizeof(int) * array->total_size);
    if (possible_array == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    array->total_size *= 2;
    array->array = possible_array;
  }
  array->array[array->real_size++] = element;
  return SUCCESS_FUNCTION_RETURN;
}

void free_int_array(int_array* array) {
  free(array->array);
}

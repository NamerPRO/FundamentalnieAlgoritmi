#include "carray.h"

employee** get_employee_array(employee_array* array) {
  return array->array;
}

unsigned long int get_employee_array_size(employee_array* array) {
  return array->real_size;
}

int create_employee_array(employee_array* array) {
  array->array = (employee**)malloc(sizeof(employee*) * STANDART_ARRAY_SIZE);
  if (array->array == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  array->total_size = STANDART_ARRAY_SIZE;
  array->real_size = 0;
  return SUCCESS_FUNCTION_RETURN;
}

int insert_employee_in_array(employee_array* array, employee* element) {
  if (array->real_size == array->total_size) {
    employee** possible_array = (employee**)realloc(array->array, 2 * sizeof(employee*) * array->total_size);
    if (possible_array == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    array->total_size *= 2;
    array->array = possible_array;
  }
  array->array[array->real_size++] = element;
  return SUCCESS_FUNCTION_RETURN;
}

// void free_employee_array(employee_array* array) {
//   free(array->array);
// }

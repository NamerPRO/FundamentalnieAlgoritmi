#include "carray.h"

mail** get_mail_array(mail_array* array) {
  return array->array;
}

unsigned long int get_mail_array_size(mail_array* array) {
  return array->real_size;
}

int create_mail_array(mail_array* array) {
  array->array = (mail**)malloc(sizeof(mail*) * STANDART_ARRAY_SIZE);
  if (array->array == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  array->total_size = STANDART_ARRAY_SIZE;
  array->real_size = 0;
  return SUCCESS_FUNCTION_RETURN;
}

int insert_mail_in_array(mail_array* array, mail* element) {
  if (array->real_size == array->total_size) {
    mail** possible_array = (mail**)realloc(array->array, 2 * sizeof(mail*) * array->total_size);
    if (possible_array == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    array->total_size *= 2;
    array->array = possible_array;
  }
  array->array[array->real_size++] = element;
  return SUCCESS_FUNCTION_RETURN;
}

// void free_mail_array(mail_array* array) {
//   free(array->array);
// }

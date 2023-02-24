#include <stdio.h>
#include <ctype.h>

#include "cio.h"
#include "cinteger.h"
#include "coverflow.h"
#include "cstring.h"
#include "carray.h"

#define EXIT_KEYWORD "STOP"

int get_one_bits(int number) {
  int count = 0;
  do {
    if (number & 1 == 1) {
      ++count;
    }
  } while (number = (number >> 1));
  return count;
}

int get_one_consecutive_bits(int number) {
  int max_count = 0;
  int count = 0;
  do {
    if (number & 1 == 1) {
      ++count;
    } else {
      max_count = (max_count > count) ? max_count : count;
      count = 0;
    }
  } while (number = (number >> 1));
  return count;
}

int sort_comporator(const void * x1, const void * x2) {
  return *(int*)x1 - *(int*)x2;
}

void search_by_one_bits(int k, int** result_arr, unsigned long int* size, int* status, int (*counter)(int x)) {
  string digit_str, exit_str;
  if (create_empty_string(&digit_str) == MEMORY_ALLOCATE_EXCEPTION) {
    *status = MEMORY_ALLOCATE_EXCEPTION;
    return;
  }
  char* ptr_str;
  if ((ptr_str = str(EXIT_KEYWORD)) == NULL) {
    free_string(&digit_str);
    *status = MEMORY_ALLOCATE_EXCEPTION;
    return;
  }
  create_string(&exit_str, ptr_str);
  int_array result_int_arr;
  if (create_int_array(&result_int_arr) == MEMORY_ALLOCATE_EXCEPTION) {
    free_string(&digit_str);
    free_string(&exit_str);
    *status = MEMORY_ALLOCATE_EXCEPTION;
    return;
  }
  int execute_status;
  while ((execute_status = read_string(stdin, &digit_str, isspace)) == SUCCESS_FUNCTION_RETURN) {
    if (!string_compare(&digit_str, &exit_str, standart_string_comporator)) {
      break;
    }
    int digit = get_digit_from_string(&digit_str, &execute_status);
    if (execute_status == NAN || execute_status == OVERFLOW) {
      break;
    }
    if (get_one_bits(digit) <= k) {
      if ((execute_status = insert_int_in_array(&result_int_arr, digit)) == MEMORY_ALLOCATE_EXCEPTION) {
        break;
      }
    }
  }
  free_string(&digit_str);
  free_string(&exit_str);
  switch (execute_status) {
    case MEMORY_ALLOCATE_EXCEPTION:
      free_int_array(&result_int_arr);
      *status = MEMORY_ALLOCATE_EXCEPTION;
      return;
    case NAN:
      free_int_array(&result_int_arr);
      *status = NAN;
      return;
    case OVERFLOW:
      free_int_array(&result_int_arr);
      *status = OVERFLOW;
      return;
  }
  *result_arr = get_int_array(&result_int_arr);
  *size = get_int_array_size(&result_int_arr);
  *status = SUCCESS_FUNCTION_RETURN;
  qsort(*result_arr, *size, sizeof(int), sort_comporator);
}

int main() {
  int* arr;
  unsigned long int size;
  printf("k=");
  int k, status = safe_read(stdin, "%d", isspace, &k);
  if (status != SUCCESS_FUNCTION_RETURN) {
    return status;
  }
  search_by_one_bits(k, &arr, &size, &status, get_one_bits);
  if (status == SUCCESS_FUNCTION_RETURN) {
    printf("Output array looks like:\n");
    for (unsigned long int i = 0; i < size; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);
  } else {
    printf("An error occured while performing given operation.\nError code: %d\n", status);
  }
  printf("k=");
  status = safe_read(stdin, "%d", isspace, &k);
  if (status != SUCCESS_FUNCTION_RETURN) {
    return status;
  }
  search_by_one_bits(k, &arr, &size, &status, get_one_consecutive_bits);
  if (status == SUCCESS_FUNCTION_RETURN) {
    printf("Output array looks like:\n");
    for (unsigned long int i = 0; i < size; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);
  } else {
    printf("An error occured while performing given operation.\nError code: %d\n", status);
  }
  return 0;
}

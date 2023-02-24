#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "cinteger.h"
#include "coverflow.h"
#include "cstring.h"

#define CONVERTATION_EXCEPTION 1
#define OVERFLOW_EXCEPTION 2
#define NAN_EXCEPTION 3
#define INPUT_EXCEPTION 4

typedef struct cnumber {
  char* real_start;
  char* data_start;
} cnumber;

char to_system(int symbol) {
  return (symbol < 10) ? symbol + '0' : symbol - 10 + 'A';
}

cnumber convert(int number, int r) {
  int negative_flag = 1;
  if (number < 0) {
    negative_flag = -1;
  }
  number *= negative_flag;
  char* buffer = (char*)malloc(sizeof(char) * STANDART_STRING_SIZE);
  if (buffer == NULL) {
    return (cnumber){ .real_start = NULL, .data_start = NULL };
  }
  char* buffer_ptr = buffer + STANDART_STRING_SIZE - 1;
  *buffer_ptr = '\0';
  int mask = (1 << r) - 1;
  do {
    *--buffer_ptr = to_system(number & mask);
  } while (number = (number >> r));
  if (negative_flag == -1) {
    *--buffer_ptr = '-';
  }
  return (cnumber){ .real_start = buffer, .data_start = buffer_ptr };
}

char* get_number(cnumber number) {
  return number.data_start;
}

int has_number(cnumber number) {
  return (number.real_start == NULL) ? 0 : 1;
}

void free_number(cnumber number) {
  free(number.real_start);
}

int main() {
  printf("Enter a number and r:");

  string str_number;
  if (create_string(&str_number) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  if (read_string(&str_number, isspace) == MEMORY_ALLOCATE_EXCEPTION) {
    free_string(str_number);
    return MEMORY_ALLOCATE_EXCEPTION;
  }

  char* str_r;
  if (create_string(&str_r) == MEMORY_ALLOCATE_EXCEPTION) {
    free_string(str_number);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  if (read_string(&str_r, isspace) == MEMORY_ALLOCATE_EXCEPTION) {
    free_string(str_number);
    free_string(str_r);
    return MEMORY_ALLOCATE_EXCEPTION;
  }

  int number, r, status;
  number = get_digit_from_string(str_number, &status);
  free_string(str_number);
  if (status == OVERFLOW) {
    free_string(str_r);
    return OVERFLOW_EXCEPTION;
  }
  if (status == NAN) {
    free_string(str_r);
    return NAN_EXCEPTION;
  }
  r = get_digit_from_string(str_r, &status);
  free_string(str_r);
  if (status == OVERFLOW) {
    return OVERFLOW_EXCEPTION;
  }
  if (status == NAN) {
    return NAN_EXCEPTION;
  }
  if (r < 1 || r > 5) {
    return INPUT_EXCEPTION;
  }

  cnumber converted_number = convert(number, r);
  if (has_number(converted_number)) {
    printf("Converted number = %s\n", get_number(converted_number));
    free_number(converted_number);
  } else {
    return CONVERTATION_EXCEPTION;
  }
  return 0;
}

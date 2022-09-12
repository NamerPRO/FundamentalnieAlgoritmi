#include <stdio.h>

#include "decimal.h"
#include "overflow.h"

#define ARGUMENTS_EXCEPTION 1
#define FILE_EXCEPTION 2
#define NUMBER_EXCEPTION 3
#define OVERFLOW_EXCEPTION 4

int max(const int a, const int b) {
  return (a > b) ? a : b;
}

int safe_get_minimal_system(char* number, int* is_number) {
  int max_digit = 0;
  int digit_to_int = 0;
  *is_number = 1;
  for (int i = 0; number[i] != '\0'; ++i) {
    digit_to_int = get_decimal_value(number[i]);
    if (digit_to_int == -1) {
      *is_number = 0;
      return -1;
    }
    max_digit = max(digit_to_int, max_digit);
  }
  if (max_digit < 2) {
    return 2;
  }
  return max_digit + 1;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }

  FILE* input_file = NULL;
  if ((input_file = fopen(argv[1], "r")) == NULL) {
    return FILE_EXCEPTION;
  }

  FILE* output_file = NULL;
  if ((output_file = fopen("./output_file", "w")) == NULL) {
    return FILE_EXCEPTION;
  }

  char number[255];
  int is_flagged;
  int sys;
  int decimal_number;
  while (fscanf(input_file, "%s", number) == 1) {
    sys = safe_get_minimal_system(number, &is_flagged);
    if (!is_flagged) {
      return NUMBER_EXCEPTION;
    }
    decimal_number = safe_to_dec(number, sys, &is_flagged);
    if (is_flagged) {
      return OVERFLOW_EXCEPTION;
    }
    fprintf(output_file, "%s %d %d\n", number, sys, decimal_number);
  }

  return 0;
}

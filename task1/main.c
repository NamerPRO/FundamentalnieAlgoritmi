#include <stdio.h>
#include <string.h>
#include <math.h>

#include "overflow.h"

#define WRONG_INPUT_ARGS 1
#define WRONG_ARGUMENT_EXCEPTION 2
#define UNKNOWN_FLAG_EXCEPTION 3
#define VARIABLE_OVERFLOW_EXCEPTION 4

typedef enum FLAG_TYPE {
  H,
  P,
  S,
  E,
  A,
  F,
  UNKNOWN
} FLAG_TYPE;

int is_symbol_digit(char c) {
  if (c >= '0' && c <= '9') {
    return 1;
  }
  return 0;
}

unsigned int get_digit_from_string(char* str, int* is_overflowed_input) {
  unsigned int number = 0;
  for (unsigned int i = 0; i < strlen(str) && !*is_overflowed_input; ++i) {
    if (!is_symbol_digit(str[i])) {
      return 0;
    }
    number = safe_mult(number, 10, is_overflowed_input);
    if (*is_overflowed_input) {
      break;
    }
    number = safe_sum(number, str[i] - '0', is_overflowed_input);
  }
  return number;
}

FLAG_TYPE get_flag_type(char* str) {
  if (!strcmp(str, "-h") || !strcmp(str, "/h")) {
    return H;
  }
  if (!strcmp(str, "-p") || !strcmp(str, "/p")) {
    return P;
  }
  if (!strcmp(str, "-s") || !strcmp(str, "/s")) {
    return S;
  }
  if (!strcmp(str, "-e") || !strcmp(str, "/e")) {
    return E;
  }
  if (!strcmp(str, "-a") || !strcmp(str, "/a")) {
    return A;
  }
  if (!strcmp(str, "-f") || !strcmp(str, "/f")) {
    return F;
  }
  return UNKNOWN;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    //printf("WRONG_INPUT_ARGS\n");
    return WRONG_INPUT_ARGS;
  }
  int is_overflowed_input = 0;
  unsigned int number = get_digit_from_string(argv[1], &is_overflowed_input);
  if (is_overflowed_input) {
    //printf("Too large value was given as an argument :( Cannot process!\n");
    return VARIABLE_OVERFLOW_EXCEPTION;
  }
  if (number == 0) {
    //printf("WRONG_ARGUMENT_EXCEPTION\n");
    return WRONG_ARGUMENT_EXCEPTION;
  }
  FLAG_TYPE flag = get_flag_type(argv[2]);
  switch (flag) {
  case H:
    printf("Numbers multiples %d within limit from 1 to 100 are: ", number);
    int required_numbers_exist = 0;
    for (unsigned int i = 1; i <= 100; ++i) {
      if (i % number == 0) {
        printf("%u ", i);
        required_numbers_exist = 1;
      }
    }
    if (!required_numbers_exist) {
      printf("no such numbers :(");
    }
    printf("\n");
    break;
  case P: ;
    int is_number_prime = 1;
    for (unsigned int i = 2; i <= (unsigned int)sqrt(number); ++i) {
      if (number % i == 0) {
        is_number_prime = 0;
        break;
      }
    }
    if (is_number_prime) {
      printf("Number is prime!\n");
    } else {
      printf("Number is composite!\n");
    }
    break;
  case S:
    printf("Splitted number is: ");
    for (unsigned int i = 0; i < strlen(argv[1]); ++i) {
      printf("%c ", argv[1][i]);
    }
    printf("\n");
    break;
  case E:
    if (number > 10) {
      //printf("WRONG_ARGUMENT_EXCEPTION\n");
      return WRONG_ARGUMENT_EXCEPTION;
    }
    for (unsigned int i = 1; i <= 10; ++i) {
      printf("For number %u: ", i);
      long long number_to_pow = 1;
      for (unsigned int j = 1; j <= number; ++j) {
        number_to_pow *= i;
        printf("%lld ", number_to_pow);
      }
      printf("\n");
    }
    break;
  case A: ;
    unsigned int sum = 0;
    int is_overflowed_sum = 0;
    for (unsigned int i = 1; i <= number && !is_overflowed_sum; ++i) {
      sum = safe_sum(sum, i, &is_overflowed_sum);
    }
    if (is_overflowed_sum) {
      //printf("There was an overflow while calculating sum from 1 to %u :(\n", number);
      return VARIABLE_OVERFLOW_EXCEPTION;
    } else {
      printf("Sum for all numbers from 1 to %u is %u\n", number, sum);
    }
    break;
  case F: ;
    unsigned int factorial = 1;
    int is_overflowed_factorial = 0;
    for (unsigned int i = 2; i <= number && !is_overflowed_factorial; ++i) {
      factorial = safe_mult(factorial, i, &is_overflowed_factorial);
    }
    if (is_overflowed_factorial) {
      //printf("There was an overflow while calculating factorial of %u :(\n", number);
      return VARIABLE_OVERFLOW_EXCEPTION;
    } else {
      printf("Factorial of a number %u is %u\n", number, factorial);
    }
    break;
  case UNKNOWN:
    //printf("UNKNOWN_FLAG_EXCEPTION\n");
    return UNKNOWN_FLAG_EXCEPTION;
  }
  return 0;
}

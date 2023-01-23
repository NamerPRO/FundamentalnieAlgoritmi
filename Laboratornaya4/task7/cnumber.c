#include "cnumber.h"

int is_symbol_digit(char symbol) {
  return (symbol >= '0') && (symbol <= '9');
}

double get_double_from_string(string* digit_str, int* execute_status) {
  if (!string_is_double(digit_str)) {
    *execute_status = NAN;
    return 0;
  }
  int negative_flag = 1;
  unsigned long int i = 0;
  if (digit_str->str[0] == '-') {
    negative_flag = -1;
    ++i;
  }
  double number = 0;
  for (; digit_str->str[i] != '.' && digit_str->str[i]; ++i) {
    number = safe_mult((unsigned long int)number, 10, execute_status);
    if (*execute_status == OVERFLOW) {
      return 0;
    }
    number = safe_sum((unsigned long int)number, digit_str->str[i] - '0', execute_status);
    if (*execute_status == OVERFLOW) {
      return 0;
    }
  }
  if (digit_str->str[i]) {
    ++i;
    double k = 0.1;
    for (; digit_str->str[i]; ++i) {
      number += (digit_str->str[i] - '0') * k;
      k *= 0.1;
    }
  }
  *execute_status = NUMBER;
  return number * negative_flag;
}

int get_integer_from_string(string* digit_str, int* execute_status) {
  char* str = get_raw_string(digit_str);
  int negative_flag = 1;
  int i = 0;
  if (str[0] == '-') {
    negative_flag = -1;
    ++i;
  }
  int number = 0;
  for (; i < (int)strlen(str); ++i) {
    if (!is_symbol_digit(str[i])) {
      *execute_status = NAN;
      return 0;
    }
    number = safe_mult(number, 10, execute_status);
    if (*execute_status == OVERFLOW) {
      return 0;
    }
    number = safe_sum(number, str[i] - '0', execute_status);
    if (*execute_status == OVERFLOW) {
      return 0;
    }
  }
  if (*execute_status != OVERFLOW) {
    *execute_status = NUMBER;
  }
  return negative_flag * number;
}

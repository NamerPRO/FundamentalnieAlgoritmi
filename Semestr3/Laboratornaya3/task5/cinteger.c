#include "cinteger.h"

int is_symbol_digit(char symbol) {
  return (symbol >= '0') && (symbol <= '9');
}

int get_digit_from_string(string* digit_str, int* execute_status) {
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
      break;
    }
    number = safe_sum(number, str[i] - '0', execute_status);
    if (*execute_status == OVERFLOW) {
      break;
    }
  }
  if (*execute_status != OVERFLOW) {
    *execute_status = NUMBER;
  }
  return negative_flag * number;
}

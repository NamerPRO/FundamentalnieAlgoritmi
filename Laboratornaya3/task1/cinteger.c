#include "cinteger.h"

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

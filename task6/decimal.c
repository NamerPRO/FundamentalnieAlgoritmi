#include "decimal.h"

int safe_to_dec(char* num, int sys, int* is_flagged) {
  int dec_result = 0;
  *is_flagged = 0;
  int power = safe_pow(sys, strlen(num) - 1, is_flagged);
  if (*is_flagged) {
    return -1;
  }
  int digit_to_int;
  int pre_result;
  for (int i = 0; num[i] != '\0' && !*is_flagged; ++i) {
    digit_to_int = get_decimal_value(num[i]);
    if (digit_to_int == -1) {
      *is_flagged = 1;
      return -1;
    }
    pre_result = safe_mult(power, digit_to_int, is_flagged);
    if (*is_flagged) {
      return -1;
    }
    dec_result = safe_sum(dec_result, pre_result, is_flagged);
    if (*is_flagged) {
      return -1;
    }
    power /= sys;
  }
  return dec_result;
}

int get_decimal_value(char symbol) {
  if (isdigit(symbol)) {
    return symbol - '0';
  }
  if (isalpha(symbol)) {
    return tolower(symbol) - 87;
  }
  return -1;
}

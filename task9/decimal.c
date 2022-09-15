#include "decimal.h"

#define SIZE 255

int safe_to_dec(char* num, int sys, int* is_flagged) {
  int i = 0;
  int negative_flag = 1;
  if (*num == '-') {
    negative_flag = -1;
    ++i;
  }
  int dec_result = 0;
  *is_flagged = 0;
  int power = safe_pow(sys, strlen(num) - 1 - i, is_flagged);
  if (*is_flagged) {
    return -1;
  }
  int digit_to_int;
  int pre_result;
  for (; num[i] != '\0' && !*is_flagged; ++i) {
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
  return dec_result * negative_flag;
}

char* dec_to_system(int number, int system, char* result_number) {
  int negative_flag = 1;
  if (number < 0) {
    negative_flag = -1;
    number = -number;
  }
  char* index = result_number + SIZE - 1;
  *index = '\0';
  int reminder;
  do {
    reminder = number % system;
    *--index = (reminder > 9) ? 'a' + reminder - 10 : reminder + '0';
  } while (number /= system);
  if (negative_flag == -1) {
    *--index = '-';
  }
  return index;
}

int is_number_in_required_system(char* number, int system) {
  int i = 0;
  if (*number == '-') {
    ++i;
  }
  for (; number[i] != '\0'; ++i) {
    if (system <= 10) {
        if (number[i] < '0' || number[i] >= '0' + system) {
          return 0;
        }
    } else if (!isdigit(number[i])) {
      if (number[i] < 'a' || number[i] >= 'a' + system) {
        return 0;
      }
    }
  }
  return 1;
}

int get_decimal_value(char symbol) {
  if (isdigit(symbol)) {
    return symbol - '0';
  }
  if (isalpha(symbol)) {
    return tolower(symbol) - 'a' + 10;
  }
  return -1;
}

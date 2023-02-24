#include "date.h"

int standart_date_comporator(string* str1, string* str2) {
  date dt1, dt2;
  get_date(str1, &dt1);
  get_date(str2, &dt2);
  if (dt1.year > dt2.year) {
    return 1;
  }
  if (dt1.year < dt2.year) {
    return -1;
  }
  if (dt1.month > dt2.month) {
    return 1;
  }
  if (dt1.month < dt2.month) {
    return -1;
  }
  if (dt1.day > dt2.day) {
    return 1;
  }
  if (dt1.day < dt2.day) {
    return -1;
  }
  if (dt1.hours > dt2.hours) {
    return 1;
  }
  if (dt1.hours < dt2.hours) {
    return -1;
  }
  if (dt1.minutes > dt2.minutes) {
    return 1;
  }
  if (dt1.minutes < dt2.minutes) {
    return -1;
  }
  if (dt1.seconds > dt2.seconds) {
    return 1;
  }
  if (dt1.seconds < dt2.seconds) {
    return -1;
  }
  return 0;
}

int get_date(string* str_date, date* dt) {
  char* raw_str = get_raw_string(str_date);
  int state = 0;
  string str_str;
  int execute_status;
  if (create_empty_string(&str_str) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  unsigned long int sz = get_string_size(str_date);
  for (unsigned long int i = 0; i <= sz; ++i) {
    if (i == sz || raw_str[i] == '.' || raw_str[i] == ':' || raw_str[i] == ' ') {
      switch (state) {
        case 0:
          dt->day = get_digit_from_string(&str_str, &execute_status);
          if (execute_status != NUMBER) {
            free_string(&str_str);
            return execute_status;
          }
          break;
        case 1:
          dt->month = get_digit_from_string(&str_str, &execute_status);
          if (execute_status != NUMBER) {
            free_string(&str_str);
            return execute_status;
          }
          if (dt->month > 12 || dt->month < 1) {
            free_string(&str_str);
            return WRONG_INPUT_EXCEPTION;
          }
          break;
        case 2:
          dt->year = get_digit_from_string(&str_str, &execute_status);
          if (execute_status != NUMBER) {
            free_string(&str_str);
            return execute_status;
          }
          if (dt->year < 2000) {
            free_string(&str_str);
            return WRONG_INPUT_EXCEPTION;
          }
          break;
        case 3:
          dt->hours = get_digit_from_string(&str_str, &execute_status);
          if (execute_status != NUMBER) {
            free_string(&str_str);
            return execute_status;
          }
          if (dt->hours < 0 || dt->hours > 23) {
            free_string(&str_str);
            return WRONG_INPUT_EXCEPTION;
          }
          break;
        case 4:
          dt->minutes = get_digit_from_string(&str_str, &execute_status);
          if (execute_status != NUMBER) {
            free_string(&str_str);
            return execute_status;
          }
          if (dt->minutes < 0 || dt->minutes > 59) {
            free_string(&str_str);
            return WRONG_INPUT_EXCEPTION;
          }
          break;
        case 5:
          dt->seconds = get_digit_from_string(&str_str, &execute_status);
          if (execute_status != NUMBER) {
            free_string(&str_str);
            return execute_status;
          }
          if (dt->seconds < 0 || dt->seconds > 59) {
            free_string(&str_str);
            return WRONG_INPUT_EXCEPTION;
          }
          break;
      }
      ++state;
      string_assign(&str_str, "");
      continue;
    }
    string_concat("%c", &str_str, raw_str[i]);
  }
  free_string(&str_str);
  if (state != 6) {
    return WRONG_INPUT_EXCEPTION;
  }
  return SUCCESS_FUNCTION_RETURN;
}

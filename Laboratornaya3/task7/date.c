#include "date.h"

int standart_date_comporator(string* str1, string* str2) {
  date dt1, dt2;
  validate_date(str1, &dt1);
  validate_date(str2, &dt2);
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
  return 0;
}

int validate_date(string* str_date, date* dt) {
  char* raw_str = get_raw_string(str_date);
  int state = 0;
  string str_str;
  int execute_status;
  if (create_empty_string(&str_str) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  unsigned long int sz = get_string_size(str_date);
  int day = -1, month = -1, year = -1;
  for (unsigned long int i = 0; i <= sz; ++i) {
    if (i == sz || raw_str[i] == '.') {
      switch (state) {
        case 0:
          day = get_integer_from_string(&str_str, &execute_status);
          if (execute_status != NUMBER) {
            free_string(&str_str);
            return execute_status;
          }
          if (day < 1) {
            return WRONG_INPUT_EXCEPTION;
          }
          break;
        case 1:
          month = get_integer_from_string(&str_str, &execute_status);
          if (execute_status != NUMBER) {
            free_string(&str_str);
            return execute_status;
          }
          if (month > 12 || month < 1) {
            free_string(&str_str);
            return WRONG_INPUT_EXCEPTION;
          }
          break;
        case 2:
          year = get_integer_from_string(&str_str, &execute_status);
          if (execute_status != NUMBER) {
            free_string(&str_str);
            return execute_status;
          }
          if (year < 2000) {
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
  if (day == -1 || month == -1 || year == -1) {
    return WRONG_INPUT_EXCEPTION;
  }
  if ((month == 1 || month == 3 || month == 5 ||
      month == 7 || month == 8 || month == 10 ||
      month == 12) && (day > 31)) {
    return WRONG_INPUT_EXCEPTION;
  }
  if ((month == 4 || month == 6 || month == 9
      || month == 11) && (day > 30)) {
    return WRONG_INPUT_EXCEPTION;
  }
  if (month == 2 && ((day > 28 && year % 4 != 0)
    || (day > 29 && year % 4 == 0))) {
    return WRONG_INPUT_EXCEPTION;
  }
  free_string(&str_str);
  if (dt != NULL) {
    dt->day = day;
    dt->month = month;
    dt->year = year;
  }
  return SUCCESS_FUNCTION_RETURN;
}

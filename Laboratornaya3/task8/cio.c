#include "cio.h"

int safe_read(FILE* stream, unsigned long int* count, char* pattern, int (*cmp)(int x), ...) {
  int execute_status;
  *count = 0;
  va_list args;
  va_start(args, cmp);
  string str_str;
  if ((execute_status = create_empty_string(&str_str)) != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  for (int i = 1; pattern[i - 1]; i += 2) {
    if ((execute_status = read_string(stream, &str_str, cmp)) != SUCCESS_FUNCTION_RETURN) {
      free_string(&str_str);
      return execute_status;
    }
    int status;
    int integer;
    string* string_ptr;
    char* char_ptr;
    switch (pattern[i]) {
      case 'd':
        integer = get_integer_from_string(&str_str, &status);
        if (status == NAN) {
          free_string(&str_str);
          return NAN;
        }
        if (status == OVERFLOW) {
          free_string(&str_str);
          return OVERFLOW;
        }
        int* integer_ptr = va_arg(args, int*);
        *integer_ptr = integer;
        break;
      case 's':
        string_ptr = va_arg(args, string*);
        if ((execute_status = create_string_duplicate(&str_str, string_ptr)) != SUCCESS_FUNCTION_RETURN) {
          free_string(&str_str);
          return execute_status;
        }
        break;
      case 'c':
        char_ptr = va_arg(args, char*);
        *char_ptr = str_str.str[0];
        break;
    }
    ++*count;
  }
  free_string(&str_str);
  return SUCCESS_FUNCTION_RETURN;
}

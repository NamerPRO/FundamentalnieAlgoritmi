#include "cio.h"

int safe_read(FILE* stream, unsigned long int* count, char* pattern, int (*cmp)(int x), ...) {
  *count = 0;
  va_list args;
  va_start(args, cmp);
  string str_str;
  if (create_empty_string(&str_str) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (int i = 1; pattern[i - 1]; i += 2) {
    if (read_string(stream, &str_str, cmp) == MEMORY_ALLOCATE_EXCEPTION) {
      free_string(&str_str);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    int status;
    int integer;
    string* string_ptr;
    char* char_ptr;
    switch (pattern[i]) {
      case 'd':
        integer = get_digit_from_string(&str_str, &status);
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
        if (create_string_duplicate(&str_str, string_ptr) == MEMORY_ALLOCATE_EXCEPTION) {
          free_string(&str_str);
          return MEMORY_ALLOCATE_EXCEPTION;
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

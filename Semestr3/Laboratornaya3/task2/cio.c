#include "cio.h"

int safe_read(FILE* stream, char* line, int (*cmp)(int x), ...) {
  va_list args;
  va_start(args, cmp);
  string str;
  if (create_empty_string(&str) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (int i = 1; line[i - 1]; i += 2) {
    if (read_string(stream, &str, cmp) == MEMORY_ALLOCATE_EXCEPTION) {
      free_string(&str);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    int status;
    int integer;
    string* string_ptr;
    switch (line[i]) {
      case 'd':
        integer = get_digit_from_string(&str, &status);
        if (status == NAN) {
          free_string(&str);
          return NAN;
        }
        if (status == OVERFLOW) {
          free_string(&str);
          return OVERFLOW;
        }
        int* integer_ptr = va_arg(args, int*);
        *integer_ptr = integer;
        break;
      case 's':
        string_ptr = va_arg(args, string*);
        create_string_duplicate(&str, string_ptr);
        break;
    }
  }
  free_string(&str);
  return SUCCESS_FUNCTION_RETURN;
}

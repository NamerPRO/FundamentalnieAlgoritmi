#include "cfile.h"

int standart_string_end(char symbol) {
  return isspace(symbol) ? 1 : 0;
}

int file_read_until(FILE* file, string** str_str, int (*cmp)(char symbol), int string_type) {
  int execute_status;
  char symbol;
  if (string_type == EXISTING_STRING) {
    string_assign(*str_str, "");
  } else if (string_type == NEW_STRING) {
    *str_str = (string*)malloc(sizeof(string));
    if (*str_str == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if ((execute_status = create_empty_string(*str_str)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  } else {
    return UNKNOWN_FLAG_EXCEPTION;
  }
  while ((symbol = fgetc(file)) != EOF) {
    if (cmp(symbol)) {
      return SUCCESS_FUNCTION_RETURN;
    }
    if ((execute_status = string_concat("%c", *str_str, symbol)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  free_string(*str_str);
  return EOF;
}

int file_read_n_until(FILE* file, string** str_str, int count, int (*cmp)(char symbol), int string_type) {
  int execute_status;
  unsigned long int i = 0;
  while (count--) {
    if ((execute_status = file_read_until(file, str_str + i, cmp, string_type)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    ++i;
  }
  return SUCCESS_FUNCTION_RETURN;
}

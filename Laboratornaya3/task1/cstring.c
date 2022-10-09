#include "cstring.h"

int create_string(string* str) {
  *str = (string)malloc(sizeof(char) * STANDART_STRING_SIZE);
  if (*str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  return SUCCESS_FUNCTION_RETURN;
}

int read_string(string* str, int (*cmp)(int x)) {
  char symbol;
  int max_symbols_in_str = STANDART_STRING_SIZE;
  int current_symbols_count = 0;
  while (1) {
    symbol = getchar();
    if (cmp(symbol)) {
      break;
    }
    if (current_symbols_count == max_symbols_in_str) {
      max_symbols_in_str *= 2;
      string possible_str = (string)realloc(*str, sizeof(char) * max_symbols_in_str);
      if (possible_str == NULL) {
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      *str = possible_str;
    }
    (*str)[current_symbols_count++] = symbol;
  }
  (*str)[current_symbols_count] = '\0';
  return SUCCESS_FUNCTION_RETURN;
}

void free_string(string str) {
  free(str);
}

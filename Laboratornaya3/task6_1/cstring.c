#include "cstring.h"

void create_string(string* str, char* value) {
  str->str = value;
  str->size = STANDART_STRING_SIZE * (strlen(value) / STANDART_STRING_SIZE + 1);
}

char* str(char* value) {
  unsigned long int size = strlen(value);
  char* dinamic_str = (char*)malloc(sizeof(char) * STANDART_STRING_SIZE);
  if (dinamic_str == NULL) {
    return NULL;
  }
  for (unsigned long int i = 0; i < size; ++i) {
    dinamic_str[i] = value[i];
  }
  dinamic_str[size] = '\0';
  return dinamic_str;
}

int create_empty_string(string* str) {
  str->str = (char*)malloc(sizeof(char) * STANDART_STRING_SIZE);
  if (str->str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  str->size = STANDART_STRING_SIZE;
  return SUCCESS_FUNCTION_RETURN;
}

int read_string(FILE* stream, string* str, int (*cmp)(int x)) {
  char symbol;
  unsigned long int max_symbols_in_str = STANDART_STRING_SIZE;
  unsigned long int current_symbols_count = 0;
  while (1) {
    symbol = getc(stream);
    if (cmp(symbol)) {
      break;
    }
    if (current_symbols_count == max_symbols_in_str - 1) {
      max_symbols_in_str *= 2;
      char* possible_str = (char*)realloc(str->str, sizeof(char) * max_symbols_in_str);
      if (possible_str == NULL) {
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      str->str = possible_str;
    }
    str->str[current_symbols_count++] = symbol;
  }
  str->str[current_symbols_count] = '\0';
  str->size = max_symbols_in_str;
  return SUCCESS_FUNCTION_RETURN;
}

unsigned long int get_string_size(string* str) {
  unsigned long int i = 0;
  for (; str->str[i]; ++i) {}
  return i;
}

char* get_trimmed_string(string* str, int* execute_status) {
  char* output = (char*)malloc(sizeof(char) * (get_string_size(str) + 1));
  if (output == NULL) {
    *execute_status = MEMORY_ALLOCATE_EXCEPTION;
    return NULL;
  }
  unsigned long int i = 0;
  char* raw_string = get_raw_string(str);
  for (; raw_string[i]; ++i) {
    output[i] = raw_string[i];
  }
  output[i] = '\0';
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return output;
}

int reset_string(string* str) {
  char* possible_str = (char*)malloc(sizeof(char) * STANDART_STRING_SIZE);
  if (possible_str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  free(str->str);
  str->str = possible_str;
  str->size = STANDART_STRING_SIZE;
  return SUCCESS_FUNCTION_RETURN;
}

void free_string(string* str) {
  free(str->str);
}

void print_string(FILE* stream, string* str) {
  fprintf(stream, "%s\n", str->str);
}

char* get_raw_string(string* str) {
  return str->str;
}

int standart_string_comporator(string* str1, string* str2) {
  unsigned long int len1 = get_string_size(str1);
  unsigned long int len2 = get_string_size(str2);
  if (len1 > len2) {
    return 1;
  }
  if (len2 > len1) {
    return -1;
  }
  for(unsigned long int i = 0; str1->str[i]; ++i) {
    if (str1->str[i] > str2->str[i]) {
      return 1;
    }
    if (str2->str[i] > str1->str[i]) {
      return -1;
    }
  }
  return 0;
}

int string_compare(string* str1, string* str2, int (*cmp)(string* str1, string* str2)) {
  return cmp(str1, str2);
}

int create_string_duplicate(string* str, string* cpy_str) {
  char* new_str = (char*)malloc(sizeof(char) * str->size);
  if (new_str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  cpy_str->str = new_str;
  unsigned int i = 0;
  for (; str->str[i]; ++i) {
    cpy_str->str[i] = str->str[i];
  }
  cpy_str->str[i] = '\0';
  cpy_str->size = str->size;
  return SUCCESS_FUNCTION_RETURN;
}

int string_concat(unsigned long int count, string* out_str, ...) {
  va_list args;
  va_start(args, out_str);
  unsigned long int current_length = get_string_size(out_str);
  unsigned long int max_symbols_in_str = out_str->size;
  for (unsigned long int i = 1; i < count; ++i) {
    string* str = va_arg(args, string*);
    for (unsigned long int j = 0; str->str[j]; ++j) {
      if (current_length == max_symbols_in_str - 1) {
        max_symbols_in_str *= 2;
        char* possible_str = (char*)realloc(out_str->str, sizeof(char) * max_symbols_in_str);
        if (possible_str == NULL) {
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        out_str->str = possible_str;
      }
      out_str->str[current_length++] = str->str[j];
    }
  }
  out_str->str[current_length] = '\0';
  out_str->size = max_symbols_in_str;
  return SUCCESS_FUNCTION_RETURN;
}

int string_copy(string* from, string* to) {
  char* backup = (char*)malloc(sizeof(char) * from->size);
  if (backup == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  free(to->str);
  to->str = backup;
  to->size = from->size;
  unsigned long int i = 0;
  for (; from->str[i]; ++i) {
    to->str[i] = from->str[i];
  }
  to->str[i] = '\0';
  return SUCCESS_FUNCTION_RETURN;
}

void remove_data_from_string(string* str, unsigned long int left_bound, unsigned long int right_bound) {
  for (unsigned long int i = right_bound + 1; str->str[i]; ++i) {
    str->str[left_bound++] = str->str[i];
  }
  str->str[left_bound] = '\0';
}

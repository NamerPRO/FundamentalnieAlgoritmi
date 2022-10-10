#include "cstring.h"

void create_string(string* str, char* value) {
  str->str = value;
  str->size = STANDART_STRING_SIZE * (strlen(value) / STANDART_STRING_SIZE + 1);
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

//передавать неинициализированный cpy_str!
//иначе утечка памяти
int create_string_duplicate(string* str, string* cpy_str) {
  char* new_str = (char*)malloc(sizeof(char) * str->size);
  if (new_str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  // free(cpy_str->str);
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
  // string* out_str = va_arg(args, &string);
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
  int i = 0;
  for (; from->str[i]; ++i) {
    to->str[i] = from->str[i];
  }
  to->str[i] = '\0';
  return SUCCESS_FUNCTION_RETURN;
}

//[left_bound; right_bound]
char* remove_data_from_string(string* str, unsigned long int left_bound, unsigned long int right_bound) {
  for (unsigned long int i = right_bound + 1; str->str[i]; ++i) {
    str->str[left_bound++] = str->str[i];
  }
  str->str[left_bound] = '\0';
}

// int create_string_array(string** str_arr) {
//   *str_arr = (string*)malloc(sizeof(string) * STANDART_ARRAY_SIZE);
//   if (*str_arr == NULL) {
//     return MEMORY_ALLOCATE_EXCEPTION;
//   }
//   for (int i = 0; i < STANDART_ARRAY_SIZE; ++i) {
//     (*str_arr)[i] = (string)malloc(sizeof(char) * STANDART_STRING_SIZE);
//     if ((*str_arr)[i] == NULL) {
//       for (int j = 0; j < i; ++j) {
//         free_string((*str_arr)[j]);
//       }
//       free(*str_arr);
//       return MEMORY_ALLOCATE_EXCEPTION;
//     }
//   }
//   return SUCCESS_FUNCTION_RETURN;
// }
//
// int free_string_array(string* str_arr) {
//   for (int i = 0; i <= )
// }

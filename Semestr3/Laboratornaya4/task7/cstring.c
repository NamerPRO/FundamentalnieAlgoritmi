#include "cstring.h"

char* str(char* value, int* execute_status) {
  unsigned long int size = strlen(value);
  unsigned long int real_string_size = STANDART_STRING_SIZE;
  char* dinamic_str = (char*)malloc(sizeof(char) * real_string_size);
  if (dinamic_str == NULL) {
    *execute_status = MEMORY_ALLOCATE_EXCEPTION;
    return NULL;
  }
  for (unsigned long int i = 0; i < size; ++i) {
    if (i == real_string_size - 1) {
      char* possible_dinamic_str = (char*)realloc(dinamic_str, sizeof(char) * real_string_size * 2);
      if (possible_dinamic_str == NULL) {
        free(dinamic_str);
        return NULL;
      }
      dinamic_str = possible_dinamic_str;
      real_string_size *= 2;
    }
    dinamic_str[i] = value[i];
  }
  dinamic_str[size] = '\0';
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return dinamic_str;
}

int create_string(string* str_str, char* value) {
  int execute_status;
  str_str->str = str(value, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  str_str->size = STANDART_STRING_SIZE * (strlen(value) / STANDART_STRING_SIZE + 1);
  return SUCCESS_FUNCTION_RETURN;
}

int create_sized_string(sized_string* str_str, char* value) {
  int execute_status;
  str_str->str = str(value, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  str_str->size = STANDART_STRING_SIZE * (strlen(value) / STANDART_STRING_SIZE + 1);
  str_str->data_size = strlen(value);
  return SUCCESS_FUNCTION_RETURN;
}

int create_all_strings(unsigned long int count, ...) {
  va_list args;
  va_start(args, count);
  int execute_status;
  for (unsigned long int i = 0; i < count; ++i) {
    string* str_str = va_arg(args, string*);
    char* value = va_arg(args, char*);
    str_str->str = str(value, &execute_status);
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    str_str->size = STANDART_STRING_SIZE * (strlen(value) / STANDART_STRING_SIZE + 1);
  }
  va_end(args);
  return SUCCESS_FUNCTION_RETURN;
}

int create_empty_sized_string(sized_string* str_str) {
  str_str->str = (char*)malloc(sizeof(char) * STANDART_STRING_SIZE);
  if (str_str->str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  str_str->str[0] = '\0';
  str_str->size = STANDART_STRING_SIZE;
  str_str->data_size = 0;
  return SUCCESS_FUNCTION_RETURN;
}

int create_empty_string(string* str_str) {
  str_str->str = (char*)malloc(sizeof(char) * STANDART_STRING_SIZE);
  if (str_str->str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  str_str->str[0] = '\0';
  str_str->size = STANDART_STRING_SIZE;
  return SUCCESS_FUNCTION_RETURN;
}

int create_all_empty_strings(unsigned long int count, ...) {
  va_list args;
  va_start(args, count);
  for (unsigned long int i = 0; i < count; ++i) {
    string* str_str = va_arg(args, string*);
    str_str->str = (char*)malloc(sizeof(char) * STANDART_STRING_SIZE);
    if (str_str->str == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    str_str->str[0] = '\0';
    str_str->size = STANDART_STRING_SIZE;
  }
  va_end(args);
  return SUCCESS_FUNCTION_RETURN;
}

int isspace_s(int symbol, int index) {
  return isspace(symbol) || symbol == '\0';
}

//Cmp function return meanings:
//zero value = include symbol into result string
//negative value = just skip symbol; continue iterating
//positive value = end iteration process
int read_string(FILE* stream, string* str, int (*cmp)(int symbol, int index), ...) {
  char symbol;
  unsigned long int max_symbols_in_str = STANDART_STRING_SIZE;
  unsigned long int current_symbols_count = 0;
  int cmp_result;
  if (stream == FROM_STRING) {
    va_list args;
    va_start(args, cmp);
    string* to_str = va_arg(args, string*);
    unsigned long int* i = va_arg(args, unsigned long int*);
    va_end(args);
    for (; symbol = str->str[*i]; ++*i) {
      cmp_result = cmp(symbol, *i);
      if (cmp_result > 0) {
        break;
      } else if (!cmp_result) {
        if (current_symbols_count == max_symbols_in_str - 1) {
          max_symbols_in_str *= 2;
          char* possible_str = (char*)realloc(to_str->str, sizeof(char) * max_symbols_in_str);
          if (possible_str == NULL) {
            return MEMORY_ALLOCATE_EXCEPTION;
          }
          to_str->str = possible_str;
        }
        to_str->str[current_symbols_count++] = symbol;
      }
    }
    ++*i;
    to_str->str[current_symbols_count] = '\0';
    to_str->size = max_symbols_in_str;
    return SUCCESS_FUNCTION_RETURN;
  }
  while (1) {
    symbol = getc(stream);
    if (symbol == EOF) {
      return EOF;
    }
    cmp_result = cmp(symbol, current_symbols_count + 1);
    if (cmp_result > 0) {
      break;
    } else if (!cmp_result) {
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
  }
  str->str[current_symbols_count] = '\0';
  str->size = max_symbols_in_str;
  return SUCCESS_FUNCTION_RETURN;
}

int read_all_strings(FILE* stream, int (*cmp)(int x), unsigned long int count, ...) {
  va_list args;
  va_start(args, count);
  int execute_status;
  for (unsigned long int i = 0; i < count; ++i) {
    string* str_str = va_arg(args, string*);
    if ((execute_status = read_string(stream, str_str, cmp)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  va_end(args);
  return SUCCESS_FUNCTION_RETURN;
}

int read_arr_strings(FILE* stream, int (*cmp)(int x), unsigned long int count, string** str_arr) {
  int execute_status;
  for (unsigned long int i = 0; i < count; ++i) {
    if ((execute_status = read_string(stream, str_arr[i], cmp)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

unsigned long int get_sized_string_size(sized_string* str) {
  return str->data_size;
}

unsigned long int get_string_size(string* str) {
  unsigned long int i = 0;
  for (; str->str[i]; ++i) {}
  return i;
}

int is_string_empty(string* str) {
  return str->str[0] == '\0';
}

int is_sized_string_empty(sized_string* str) {
  return str->str[0] == '\0';
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

void free_heap_string(string* str) {
  free(str->str);
  free(str);
}

void free_heap_sized_string(sized_string* str) {
  free(str->str);
  free(str);
}

void free_all_strings(unsigned long int count, ...) {
  va_list args;
  va_start(args, count);
  for (unsigned long int i = 0; i < count; ++i) {
    string* str = va_arg(args, string*);
    free(str->str);
  }
  va_end(args);
}

void print_string(FILE* stream, char* pattern, ...) {
  va_list args;
  va_start(args, pattern);
  int is_specifier = 0;
  for (unsigned long int i = 0; pattern[i]; ++i) {
    if (pattern[i] == '%' && !is_specifier) {
      is_specifier = 1;
      continue;
    }
    if (!is_specifier) {
      fprintf(stream, "%c", pattern[i]);
      continue;
    }
    switch (pattern[i]) {
      case 'd':
        fprintf(stream, "%d", va_arg(args, int));
        break;
      case 'p':
        fprintf(stream, "%s", va_arg(args, char*));
        break;
      case 's':
        fprintf(stream, "%s", get_raw_string(va_arg(args, string*)));
        break;
      case 'c':
        fprintf(stream, "%c", (char)va_arg(args, int));
        break;
      case 'f':
        fprintf(stream, "%lf", va_arg(args, double));
        break;
      case '%':
        fprintf(stream, "%c", '%');
        break;
    }
    is_specifier = 0;
  }
  va_end(args);
}

char* get_raw_string(string* str) {
  return str->str;
}

char* get_raw_sized_string(sized_string* str) {
  return str->str;
}

int standart_alphabet_comporator(string* str1, string* str2) {
  unsigned long int len1 = get_string_size(str1);
  unsigned long int len2 = get_string_size(str2);
  unsigned long int len3 = (len1 > len2) ? len2 : len1;
  for(unsigned long int i = 0; i < len3; ++i) {
    if (str1->str[i] > str2->str[i]) {
      return 1;
    }
    if (str2->str[i] > str1->str[i]) {
      return -1;
    }
  }
  if (len1 > len3) {
    return 1;
  }
  if (len2 > len3) {
    return -1;
  }
  return 0;
}

int standart_string_comporator(string* str1, string* str2) {
  unsigned long int len1 = get_string_size(str1);
  unsigned long int len2 = get_string_size(str2);
  if (len1 > len2) {
    return FIRST_STRING;
  }
  if (len2 > len1) {
    return SECOND_STRING;
  }
  for(unsigned long int i = 0; str1->str[i]; ++i) {
    if (str1->str[i] > str2->str[i]) {
      return FIRST_STRING;
    }
    if (str2->str[i] > str1->str[i]) {
      return SECOND_STRING;
    }
  }
  return EQUALS;
}

int not(int status_code) {
  if (status_code == MEMORY_ALLOCATE_EXCEPTION) {
    return 0;
  }
  if (status_code != EQUALS) {
    return 1;
  }
  return 0;
}

int string_compare(char* pattern, int (*cmp)(string* str1, string* str2), int* execute_status, string* str1, ...) {
  va_list arg;
  va_start(arg, str1);
  int cmp_result;
  if (pattern[1] == 's') {
    cmp_result = cmp(str1, va_arg(arg, string*));
  } else if (pattern[1] == 'p') {
    string str2;
    if (create_string(&str2, va_arg(arg, char*)) == MEMORY_ALLOCATE_EXCEPTION) {
      *execute_status = MEMORY_ALLOCATE_EXCEPTION;
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    cmp_result = cmp(str1, &str2);
    free_string(&str2);
  }
  va_end(arg);
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return cmp_result;
}

int string_multy_compare(char* pattern, int (*cmp)(string* str1, string* str2), int* execute_status, string* str1, ...) {
  va_list args;
  va_start(args, str1);
  int cmp_result;
  for (unsigned long int i = 0; ; i += 2) {
    if (pattern[i] == 'p') {
      char* str2 = va_arg(args, char*);
      cmp_result = string_compare("%p", cmp, execute_status, str1, str2);
    } else {
      string* str2 = va_arg(args, string*);
      cmp_result = string_compare("%s", cmp, execute_status, str1, str2);
    }
    if (cmp_result == MEMORY_ALLOCATE_EXCEPTION) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if (cmp_result) {
      return cmp_result;
    }
    if (pattern[i + 1] == '\0') {
      break;
    }
  }
  va_end(args);
  return 0;
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

int sized_string_concat(char* pattern, sized_string* out_str, ...) {
  va_list args;
  va_start(args, out_str);
  unsigned long int current_length = out_str->data_size;
  unsigned long int max_symbols_in_str = out_str->size;
  for (unsigned long int i = 1; pattern[i - 1]; i += 2) {
    if (pattern[i] == 'd') {
      int number = va_arg(args, int);
      int negative_flag = 0;
      if (number < 0) {
        number *= -1;
        negative_flag = 1;
      }
      int number_new = number;
      int number_size = negative_flag;
      do {
        ++number_size;
      } while ((number_new /= 10) > 0);
      if (current_length >= max_symbols_in_str - number_size) {
        max_symbols_in_str *= 2;
        char* possible_str = (char*)realloc(out_str->str, sizeof(char) * max_symbols_in_str);
        if (possible_str == NULL) {
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        out_str->str = possible_str;
      }
      int number_position = number_size;
      while (number_position--) {
        out_str->str[current_length + number_position] = number % 10 + '0';
        number /= 10;
      }
      if (negative_flag) {
        out_str->str[current_length] = '-';
      }
      current_length += number_size;
    } else if (pattern[i] == 'c') {
      if (current_length == max_symbols_in_str - 1) {
        max_symbols_in_str *= 2;
        char* possible_str = (char*)realloc(out_str->str, sizeof(char) * max_symbols_in_str);
        if (possible_str == NULL) {
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        out_str->str = possible_str;
      }
      out_str->str[current_length++] = (char)va_arg(args, int);
    } else if (pattern[i] == 'p') {
      char* str_str = va_arg(args, char*);
      for (unsigned long int j = 0; str_str[j]; ++j) {
        if (current_length == max_symbols_in_str - 1) {
          max_symbols_in_str *= 2;
          char* possible_str = (char*)realloc(out_str->str, sizeof(char) * max_symbols_in_str);
          if (possible_str == NULL) {
            return MEMORY_ALLOCATE_EXCEPTION;
          }
          out_str->str = possible_str;
        }
        out_str->str[current_length++] = str_str[j];
      }
    } else if (pattern[i] == 's') {
      string* str_str = va_arg(args, string*);
      for (unsigned long int j = 0; str_str->str[j]; ++j) {
        if (current_length == max_symbols_in_str - 1) {
          max_symbols_in_str *= 2;
          char* possible_str = (char*)realloc(out_str->str, sizeof(char) * max_symbols_in_str);
          if (possible_str == NULL) {
            return MEMORY_ALLOCATE_EXCEPTION;
          }
          out_str->str = possible_str;
        }
        out_str->str[current_length++] = str_str->str[j];
      }
    }
  }
  out_str->str[current_length] = '\0';
  out_str->size = max_symbols_in_str;
  out_str->data_size = current_length;
  va_end(args);
  return SUCCESS_FUNCTION_RETURN;
}

int string_concat(char* pattern, string* out_str, ...) {
  va_list args;
  va_start(args, out_str);
  unsigned long int current_length = get_string_size(out_str);
  unsigned long int max_symbols_in_str = out_str->size;
  for (unsigned long int i = 1; pattern[i - 1]; i += 2) {
    if (pattern[i] == 'd') {
      int number = va_arg(args, int);
      int negative_flag = 0;
      if (number < 0) {
        number *= -1;
        negative_flag = 1;
      }
      int number_new = number;
      int number_size = negative_flag;
      do {
        ++number_size;
      } while ((number_new /= 10) > 0);
      if (current_length >= max_symbols_in_str - number_size) {
        max_symbols_in_str *= 2;
        char* possible_str = (char*)realloc(out_str->str, sizeof(char) * max_symbols_in_str);
        if (possible_str == NULL) {
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        out_str->str = possible_str;
      }
      int number_position = number_size;
      while (number_position--) {
        out_str->str[current_length + number_position] = number % 10 + '0';
        number /= 10;
      }
      if (negative_flag) {
        out_str->str[current_length] = '-';
      }
      current_length += number_size;
    } else if (pattern[i] == 'c') {
      if (current_length == max_symbols_in_str - 1) {
        max_symbols_in_str *= 2;
        char* possible_str = (char*)realloc(out_str->str, sizeof(char) * max_symbols_in_str);
        if (possible_str == NULL) {
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        out_str->str = possible_str;
      }
      out_str->str[current_length++] = (char)va_arg(args, int);
    } else if (pattern[i] == 'p') {
      char* str_str = va_arg(args, char*);
      for (unsigned long int j = 0; str_str[j]; ++j) {
        if (current_length == max_symbols_in_str - 1) {
          max_symbols_in_str *= 2;
          char* possible_str = (char*)realloc(out_str->str, sizeof(char) * max_symbols_in_str);
          if (possible_str == NULL) {
            return MEMORY_ALLOCATE_EXCEPTION;
          }
          out_str->str = possible_str;
        }
        out_str->str[current_length++] = str_str[j];
      }
    } else if (pattern[i] == 's') {
      string* str_str = va_arg(args, string*);
      for (unsigned long int j = 0; str_str->str[j]; ++j) {
        if (current_length == max_symbols_in_str - 1) {
          max_symbols_in_str *= 2;
          char* possible_str = (char*)realloc(out_str->str, sizeof(char) * max_symbols_in_str);
          if (possible_str == NULL) {
            return MEMORY_ALLOCATE_EXCEPTION;
          }
          out_str->str = possible_str;
        }
        out_str->str[current_length++] = str_str->str[j];
      }
    }
  }
  out_str->str[current_length] = '\0';
  out_str->size = max_symbols_in_str;
  va_end(args);
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

void string_gnaw(string* str, unsigned long int left_bound, unsigned long int right_bound) {
  for (unsigned long int i = right_bound + 1; str->str[i]; ++i) {
    str->str[left_bound++] = str->str[i];
  }
  str->str[left_bound] = '\0';
}

char standart_random_character(unsigned long int position) {
  int symbol = rand() % 62;
  return (symbol < 10) ? symbol + '0' : (symbol < 36) ? symbol - 10 + 'A' : symbol - 36 + 'a';
}

int get_random_string(string* str, unsigned long int size, char (*rnd_char)(unsigned long int position)) {
  unsigned long int current_length = 0;
  unsigned long int max_symbols_in_str = STANDART_STRING_SIZE;
  unsigned long int i = 0;
  srand(time(NULL) + rand() % 100);
  for (; i < size; ++i) {
    if (current_length == max_symbols_in_str - 1) {
      max_symbols_in_str *= 2;
      char* possible_str = (char*)realloc(str->str, sizeof(char) * max_symbols_in_str);
      if (possible_str == NULL) {
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      str->str = possible_str;
    }
    str->str[current_length++] = rnd_char(i);
  }
  str->str[i] = '\0';
  return SUCCESS_FUNCTION_RETURN;
}

int standart_string_range(char symbol) {
  return (symbol >= 'a' && symbol <= 'z') ? 1 : (symbol >= 'A' && symbol <= 'Z') ? 1 : 0;
}

int standart_numeric_range(char symbol) {
  return (symbol >= '0' && symbol <= '9') ? 1 : 0;
}

int string_contains_only(string* str, int (*in_range)(char symbol)) {
  char* raw_str = get_raw_string(str);
  for (unsigned long int i = 0; raw_str[i]; ++i) {
    if (!in_range(raw_str[i])) {
      return 0;
    }
  }
  return 1;
}

int string_is_integer(string* str_str) {
  unsigned long int i = 0;
  if (str_str->str[0] == '-') {
    i = 1;
  }
  for (; str_str->str[i]; ++i) {
    if (!standart_numeric_range(str_str->str[i])) {
      return 0;
    }
  }
  return 1;
}

int string_is_double(string* str_str) {
  unsigned long int i = 0;
  if (str_str->str[0] == '-') {
    i = 1;
  }
  int has_dot = 0;
  for (; str_str->str[i]; ++i) {
    if (str_str->str[i] == '.') {
      if (has_dot == 1) {
        return 0;
      }
      has_dot = 1;
      continue;
    }
    if (!standart_numeric_range(str_str->str[i])) {
      return 0;
    }
  }
  return 1;
}

void to_upper_case(string* str) {
  char* str_str = get_raw_string(str);
  for (unsigned long int i = 0; str_str[i]; ++i) {
    str_str[i] = toupper(str_str[i]);
  }
}

void to_lower_case(string* str) {
  char* str_str = get_raw_string(str);
  for (unsigned long int i = 0; str_str[i]; ++i) {
    str_str[i] = tolower(str_str[i]);
  }
}

void string_reverse(string* str_str, unsigned long int lhs, unsigned long int rhs) {
  unsigned long int to = lhs + (rhs - lhs + 1) / 2;
  char* raw_str = get_raw_string(str_str);
  for (unsigned long int i = lhs; i < to; ++i) {
    char symbol = raw_str[i];
    raw_str[i] = raw_str[rhs - i + lhs];
    raw_str[rhs - i + lhs] = symbol;
  }
}

int to_string(string* str_str, char* pattern, ...) {
  int execute_status;
  va_list arg;
  va_start(arg, pattern);
  if (pattern[1] == 'p') {
    free_string(str_str);
    str_str->str = str(va_arg(arg, char*), &execute_status);
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    unsigned long int possible_size = STANDART_STRING_SIZE * (strlen(str_str->str) / STANDART_STRING_SIZE + 1);
    if (possible_size > str_str->size) {
      str_str->size = possible_size;
    }
    return SUCCESS_FUNCTION_RETURN;
  }
  if (pattern[1] == 'c') {
    str_str->str[0] = (char)va_arg(arg, int);
    str_str->str[1] = '\0';
    return SUCCESS_FUNCTION_RETURN;
  }
  if (pattern[1] == 'd') {
    int number = va_arg(arg, int);
    int negative_flag = 0;
    if (number < 0) {
      negative_flag = 1;
      number *= -1;
    }
    if (number == 0) {
      str_str->str[0] = '0';
      str_str->str[1] = '\0';
      return SUCCESS_FUNCTION_RETURN;
    }
    int i = negative_flag;
    int number_new = number;
    while (number_new /= 10) {
      ++i;
    }
    int j = i;
    while (number > 0) {
      str_str->str[i--] = number % 10 + '0';
      number /= 10;
    }
    if (negative_flag) {
      str_str->str[i] = '-';
    }
    str_str->str[j + 1] = '\0';
    return SUCCESS_FUNCTION_RETURN;
  }
}

int string_assign(string* str_str, char* str_new) {
  return to_string(str_str, "%p", str_new);
}

int sized_string_reset(sized_string* sstr) {
  sstr->data_size = 0;
  sstr->str[0] = '\0';
}

int create_string_array(string*** str_arr, unsigned long int size) {
  int execute_status;
  *str_arr = (string**)malloc(sizeof(string*) * size);
  if (*str_arr == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (unsigned long int i = 0; i < size; ++i) {
    (*str_arr)[i] = (string*)malloc(sizeof(string));
  }
  while (size--) {
    if ((execute_status = create_empty_string((*str_arr)[size])) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

void free_string_array(string*** str_arr, int size) {
  while (size--) {
    free_string((*str_arr)[size]);
    free((*str_arr)[size]);
  }
  free(*str_arr);
}

sized_string* heap_sized_string(char* c_str) {
  sized_string* heap_str = (sized_string*)malloc(sizeof(sized_string));
  if (heap_str == NULL) {
    return NULL;
  }
  if (c_str == EMPTY) {
    if (create_empty_sized_string(heap_str) != SUCCESS_FUNCTION_RETURN) {
      free(heap_str);
      return NULL;
    }
  } else {
    if (create_sized_string(heap_str, c_str) != SUCCESS_FUNCTION_RETURN) {
      free(heap_str);
      return NULL;
    }
  }
  return heap_str;
}

string* heap_string_from_char(char symbol) {
  string* heap_str = (string*)malloc(sizeof(string));
  if (heap_str == NULL) {
    return NULL;
  }
  if (create_empty_string(heap_str) != SUCCESS_FUNCTION_RETURN) {
    free(heap_str);
    return NULL;
  }
  string_concat("%c", heap_str, symbol);
  return heap_str;
}

string* heap_string(char* c_str) {
  string* heap_str = (string*)malloc(sizeof(string));
  if (heap_str == NULL) {
    return NULL;
  }
  if (c_str == EMPTY) {
    if (create_empty_string(heap_str) != SUCCESS_FUNCTION_RETURN) {
      free(heap_str);
      return NULL;
    }
  } else {
    if (create_string(heap_str, c_str) != SUCCESS_FUNCTION_RETURN) {
      free(heap_str);
      return NULL;
    }
  }
  return heap_str;
}

int read_string_to(string* str_str, unsigned long int* start, string* out_str, int (*cmp)(int symbol)) {
  int execute_status;
  for (; str_str->str[*start] && !cmp(str_str->str[*start]); ++(*start)) {
    if ((execute_status = string_concat("%c", out_str, str_str->str[*start])) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

int scompare(string* lhs, char* rhs, int* execute_status) {
  return string_compare("%p", standart_string_comporator, execute_status, lhs, rhs);
}

int scompare_s(string* lhs, string* rhs) {
  int execute_status;
  return string_compare("%s", standart_string_comporator, &execute_status, lhs, rhs);
}

char sat(string* str, unsigned long int index) {
  return str->str[index];
}

char _char(string* str) {
  return str->str[0];
}

char __char(sized_string* str) {
  return str->str[0];
}

int cin(char* symbol, int from, int to, int flag) {
  switch (flag) {
    case LL:
      return (symbol > from) ? (symbol < to) ? 1 : 0 : 0;
    case LG:
      return (symbol > from) ? (symbol <= to) ? 1 : 0 : 0;
    case GL:
      return (symbol >= from) ? (symbol < to) ? 1 : 0 : 0;
    case GG:
      return (symbol >= from) ? (symbol <= to) ? 1 : 0 : 0;
  }
}

string* clear_size(sized_string* sized_str) {
  string* str = (string*)malloc(sizeof(string));
  if (str == NULL) {
    return NULL;
  }
  str->str = sized_str->str;
  str->size = sized_str->size;
  free(sized_str);
  return str;
}

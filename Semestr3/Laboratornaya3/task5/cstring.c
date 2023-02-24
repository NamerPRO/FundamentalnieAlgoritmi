#include "cstring.h"

//если в функцию подать большую строку, то надо
//довыделять память
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

//Создать функцию с переменным числом аргументов,
//чтобы создавать сразу несколько строк
//одной строчкой кода
int create_string(string* str_str, char* value) {
  int execute_status;
  str_str->str = str(value, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  str_str->size = STANDART_STRING_SIZE * (strlen(value) / STANDART_STRING_SIZE + 1);
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
    if (symbol == EOF) {
      return EOF;
    }
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
    return -1;
  }
  if (len2 > len3) {
    return 1;
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

int string_compare(char* pattern, int (*cmp)(string* str1, string* str2), string* str1, ...) {
  va_list arg;
  va_start(arg, str1);
  int cmp_result;
  if (!strcmp(pattern, "%s\0")) {
    cmp_result = cmp(str1, va_arg(arg, string*));
  } else {
    int execute_status;
    string str2;
    create_string(&str2, str(va_arg(arg, char*), &execute_status));
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    cmp_result = cmp(str1, &str2);
    free_string(&str2);
  }
  va_end(arg);
  return cmp_result;
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
  srand(time(NULL) + position);
  int symbol = rand() % 62;
  return (symbol < 10) ? symbol + '0' : (symbol < 36) ? symbol - 10 + 'A' : symbol - 36 + 'a';
}

int get_random_string(string* str, unsigned long int size, char (*rnd_char)(unsigned long int position)) {
  unsigned long int current_length = 0;
  unsigned long int max_symbols_in_str = STANDART_STRING_SIZE;
  unsigned long int i = 0;
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

int contains_only(string* str, int (*in_range)(char symbol)) {
  char* raw_str = get_raw_string(str);
  for (unsigned long int i = 0; raw_str[i]; ++i) {
    if (!in_range(raw_str[i])) {
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

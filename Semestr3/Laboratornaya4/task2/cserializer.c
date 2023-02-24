#include "cserializer.h"

int flush_serialized_string(serialized_string* s_string, char* line, ...) {
  if (line != NULL) {
    va_list arg;
    va_start(arg, line);
    int size = va_arg(arg, int);
    va_end(arg);
    while (s_string->struct_size < size) {
      char* possible_line = (char*)realloc(s_string->line, sizeof(char) * s_string->struct_size * 2);
      if (possible_line == NULL) {
        //...
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      s_string->line = possible_line;
      s_string->struct_size *= 2;
    }
    memcpy(s_string->line, line, size);
    s_string->data_size = size;
  } else {
    s_string->data_size = 0;
  }
  return SUCCESS_FUNCTION_RETURN;
}

int pour_in_serialized_string(serialized_string* pour_in, serialized_string* from, int indent) {
  for (int i = indent; i < from->data_size; ++i) {
    if (pour_in->data_size + 1 > pour_in->struct_size) {
      char* possible_line = (char*)realloc(pour_in->line, sizeof(char) * pour_in->struct_size * 2);
      if (possible_line == NULL) {
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      pour_in->line = possible_line;
      pour_in->struct_size *= 2;
    }
    pour_in->line[pour_in->data_size++] = from->line[i];
  }
  return SUCCESS_FUNCTION_RETURN;
}

int create_serialized_string_array(serialized_string*** s_array, int size) {
  *s_array = (serialized_string**)malloc(sizeof(serialized_string*) * size);
  if (*s_array == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (int i = 0; i < size; ++i) {
    (*s_array)[i] = create_serialized_string(EMPTY);
    if ((*s_array)[i] == NULL) {
      for (int j = 0; j < i; ++j) {
        free_serialized_string((*s_array)[j]);
      }
      free(*s_array);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

void free_serialized_string_array(serialized_string** s_array, int size) {
  for (int i = 0; i < size; ++i) {
    free_serialized_string(s_array[i]);
  }
  free(s_array);
}

serialized_string* create_serialized_string(string* line, ...) {
  serialized_string* s_string = (serialized_string*)malloc(sizeof(serialized_string));
  if (s_string == NULL) {
    return NULL;
  }
  if (line != EMPTY) {
    va_list arg;
    va_start(arg, line);
    int size = va_arg(arg, int);
    va_end(arg);
    s_string->line = (char*)malloc(sizeof(char) * STANDART_SERIALIZED_STRING_SIZE * (size / STANDART_SERIALIZED_STRING_SIZE + 1));
    if (s_string->line == NULL) {
      free(s_string);
      return NULL;
    }
    memcpy(s_string->line, get_raw_string(str), size);
    s_string->data_size = size;
    s_string->struct_size = STANDART_SERIALIZED_STRING_SIZE * (size / STANDART_SERIALIZED_STRING_SIZE + 1);
  } else {
    s_string->line = (char*)malloc(sizeof(char) * STANDART_SERIALIZED_STRING_SIZE);
    if (s_string->line == NULL) {
      free(s_string);
      return NULL;
    }
    s_string->data_size = 0;
    s_string->struct_size = STANDART_SERIALIZED_STRING_SIZE;
  }
  return s_string;
}

char* get_raw_serialized_string(serialized_string* s_string) {
  return s_string->line;
}

void free_serialized_string(serialized_string* s_string) {
  free(s_string->line);
  free(s_string);
}

int serialize_char(char symbol, serialized_string* s_string) {
  if (s_string->data_size + 1 > s_string->struct_size) {
    char* possible_line = (char*)realloc(s_string->line, sizeof(char) * 2 * s_string->struct_size);
    if (possible_line == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    s_string->line = possible_line;
    s_string->struct_size *= 2;
  }
  s_string->line[s_string->data_size++] = symbol;
  return SUCCESS_FUNCTION_RETURN;
}

int serialize_integer(int number, serialized_string* s_string) {
  if (s_string->data_size + 5 > s_string->struct_size) {
    char* possible_line = (char*)realloc(s_string->line, sizeof(char) * 2 * s_string->struct_size);
    if (possible_line == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    s_string->line = possible_line;
    s_string->struct_size *= 2;
  }
  if (number < 0) {
    number = -number;
    s_string->line[s_string->data_size++] = '0';
  } else {
    s_string->line[s_string->data_size++] = '1';
  }
  s_string->data_size += 4;
  for (int i = 0; i < 4; ++i) {
    s_string->line[s_string->data_size - i - 1] = number & 255;
    number >>= 8;
  }
  return SUCCESS_FUNCTION_RETURN;
}

int serialize_double(double number, int precition, serialized_string* s_string) {
  int new_number = (int)(number * pow(10, precition));
  if (serialize_integer(new_number, s_string) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  return SUCCESS_FUNCTION_RETURN;
}

int serialize_string(char* c_string, serialized_string* s_string) {
  int string_length = strlen(c_string);
  if (serialize_integer(string_length, s_string) != SUCCESS_FUNCTION_RETURN) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (int i = 0; i < string_length; ++i) {
    if (serialize_char(c_string[i], s_string) != SUCCESS_FUNCTION_RETURN) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

char deserialize_char(serialized_string* s_string, int* index) {
  return s_string->line[(*index)++];
}

int deserialize_integer(serialized_string* s_string, int* index) {
  int result = 0;
  int is_negative = s_string->line[*index] - '0';
  ++*index;
  for (int i = 0; i < 4; ++i) {
    result = (result << 8) | (unsigned char)(s_string->line[*index]);
    ++*index;
  }
  return is_negative ? result : -result;
}

double deserialize_double(serialized_string* s_string, int precition, int* index) {
  int number = deserialize_integer(s_string, index);
  return number / pow(10, precition);
}

char* deserialize_string(serialized_string* s_string, int* index) {
  int string_length = deserialize_integer(s_string, index);
  char* c_string = (char*)malloc(sizeof(char) * string_length);
  if (c_string == NULL) {
    return NULL;
  }
  for (int i = 0; i < string_length; ++i) {
    char symbol = deserialize_char(s_string, index);
    c_string[i] = symbol;
  }
  c_string[string_length] = '\0';
  return c_string;
}

int serialize_monomial(serialized_string* s_string, monomial* element) {
  if (serialize_integer(element->coefficient, s_string) != SUCCESS_FUNCTION_RETURN) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  if (serialize_integer(element->data_size, s_string) != SUCCESS_FUNCTION_RETURN) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (int i = 0; i < element->data_size; ++i) {
    if (serialize_string(element->unknowns[i]->name, s_string) != SUCCESS_FUNCTION_RETURN) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if (serialize_integer(element->unknowns[i]->power, s_string) != SUCCESS_FUNCTION_RETURN) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

monomial* deserialize_monomial(serialized_string* s_string, int* index) {
  monomial* element = (monomial*)malloc(sizeof(monomial));
  if (element == NULL) {
    return NULL;
  }
  element->coefficient = deserialize_integer(s_string, index);
  element->data_size = deserialize_integer(s_string, index);
  element->struct_size = STANDART_MONOMIAL_SIZE * (element->data_size / STANDART_MONOMIAL_SIZE + 1);
  element->unknowns = (pair**)malloc(sizeof(pair*) * element->struct_size);
  if (element->unknowns == NULL) {
    free(element);
    return NULL;
  }
  for (int j = 0; j < element->data_size; ++j) {
    element->unknowns[j] = (pair*)malloc(sizeof(pair));
    if (element->unknowns[j] == NULL) {
      for (int k = 0; k < j; ++k) {
        free(element->unknowns[k]->name);
        free(element->unknowns[k]);
      }
      free(element->unknowns);
      free(element);
      return NULL;
    }
    if ((element->unknowns[j]->name = deserialize_string(s_string, index)) == NULL) {
      free(element->unknowns[j]);
      for (int k = 0; k < j; ++k) {
        free(element->unknowns[k]->name);
        free(element->unknowns[k]);
      }
      free(element->unknowns);
      free(element);
      return NULL;
    }
    element->unknowns[j]->power = deserialize_integer(s_string, index);
  }
  return element;
}

int serialize_polynomial(serialized_string* s_string, polynomial* element) {
  int execute_status;
  if (serialize_integer(element->monomials->size, s_string) != SUCCESS_FUNCTION_RETURN) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  list_node* node_ptr = element->monomials->head;
  for (int i = 0; i < element->monomials->size; ++i) {
    if (serialize_monomial(s_string, node_ptr->value) != SUCCESS_FUNCTION_RETURN) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    node_ptr = node_ptr->next;
  }
  if (serialize_integer(element->free_term, s_string) != SUCCESS_FUNCTION_RETURN) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  return SUCCESS_FUNCTION_RETURN;
}

int is_bracket(int symbol, int index) {
  return symbol == '(';
}

int is_comma(int symbol, int index) {
  return symbol == ',' || symbol == ')';
}

int is_brace(int symbol, int index) {
  return symbol == ',' || symbol == '}';
}

int serialize_request(serialized_string* s_string, string* request) {
  int execute_status;
  unsigned long int index = 0;
  string* unit = heap_string(EMPTY);
  if (unit == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  if ((execute_status = read_string(FROM_STRING, request, is_bracket, unit, &index)) != SUCCESS_FUNCTION_RETURN) {
    free_heap_string(unit);
    return execute_status;
  }
  if ((execute_status = serialize_string(get_raw_string(unit), s_string)) != SUCCESS_FUNCTION_RETURN) {
    free_heap_string(unit);
    return execute_status;
  }
  if (sat(request, index) == ')') {
    if ((execute_status = serialize_integer(0, s_string)) != SUCCESS_FUNCTION_RETURN) {
      free_heap_string(unit);
      return execute_status;
    }
    return SUCCESS_FUNCTION_RETURN;
  }
  if (sat(request, index) == '{') {
    if ((execute_status = serialize_integer(0, s_string)) != SUCCESS_FUNCTION_RETURN) {
      free_heap_string(unit);
      return execute_status;
    }
    ++index;
    int dots_size = 5;
    int real_dots_size = 0;
    double* dots = (double*)malloc(sizeof(double) * dots_size);
    if (dots == NULL) {
      free_heap_string(unit);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    while (sat(request, index) != ')') {
      while (sat(request, index) == ' ') {
        ++index;
      }
      if ((execute_status = read_string(FROM_STRING, request, is_brace, unit, &index)) != SUCCESS_FUNCTION_RETURN) {
        free_heap_string(unit);
        free(dots);
        return execute_status;
      }
      if (real_dots_size == dots_size) {
        double* possible_dots = (double*)realloc(dots, sizeof(double) * 2 * dots_size);
        if (possible_dots == NULL) {
          free_heap_string(unit);
          free(dots);
          return NULL;
        }
        dots = possible_dots;
        dots_size *= 2;
      }
      double coordinate = get_double_from_string(unit, &execute_status);
      if (execute_status != NUMBER) {
        free_heap_string(unit);
        free(dots);
        return execute_status;
      }
      dots[real_dots_size++] = coordinate;
    }
    if ((execute_status = serialize_integer(real_dots_size, s_string)) != SUCCESS_FUNCTION_RETURN) {
      free_heap_string(unit);
      free(dots);
      return execute_status;
    }
    for (int i = 0; i < real_dots_size; ++i) {
      if ((execute_status = serialize_double(dots[i], STANDART_EVAL_PRECITION, s_string)) != SUCCESS_FUNCTION_RETURN) {
        free_heap_string(unit);
        free(dots);
        return execute_status;
      }
    }
    free(dots);
  } else {
    if ((execute_status = read_string(FROM_STRING, request, is_comma, unit, &index)) != SUCCESS_FUNCTION_RETURN) {
      free_heap_string(unit);
      return execute_status;
    }
    while (sat(request, index) == ' ') {
      ++index;
    }
    if (sat(request, index) == ';') {
      if ((execute_status = serialize_integer(1, s_string)) != SUCCESS_FUNCTION_RETURN) {
        free_heap_string(unit);
        return execute_status;
      }
      polynomial* pol = parse_polynomial(unit);
      if (pol == NULL) {
        free_heap_string(unit);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      if ((execute_status = serialize_polynomial(s_string, pol)) != SUCCESS_FUNCTION_RETURN) {
        free_heap_string(unit);
        free_polynomial(pol);
        return execute_status;
      }
      free_polynomial(pol);
      if ((execute_status = serialize_integer(0, s_string)) != SUCCESS_FUNCTION_RETURN) {
        free_heap_string(unit);
        return execute_status;
      }
    } else {
      if (sat(request, index) == '{') {
        if ((execute_status = serialize_integer(1, s_string)) != SUCCESS_FUNCTION_RETURN) {
          free_heap_string(unit);
          return execute_status;
        }
        polynomial* pol = parse_polynomial(unit);
        if (pol == NULL) {
          free_heap_string(unit);
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        if ((execute_status = serialize_polynomial(s_string, pol)) != SUCCESS_FUNCTION_RETURN) {
          free_heap_string(unit);
          free_polynomial(pol);
          return execute_status;
        }
        free_polynomial(pol);
        ++index;
        int dots_size = 5;
        int real_dots_size = 0;
        double* dots = (double*)malloc(sizeof(double) * dots_size);
        if (dots == NULL) {
          free_heap_string(unit);
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        while (sat(request, index) != ')') {
          while (sat(request, index) == ' ') {
            ++index;
          }
          if ((execute_status = read_string(FROM_STRING, request, is_brace, unit, &index)) != SUCCESS_FUNCTION_RETURN) {
            free_heap_string(unit);
            free(dots);
            return execute_status;
          }
          if (real_dots_size == dots_size) {
            double* possible_dots = (double*)realloc(dots, sizeof(double) * 2 * dots_size);
            if (possible_dots == NULL) {
              free_heap_string(unit);
              free(dots);
              return NULL;
            }
            dots = possible_dots;
            dots_size *= 2;
          }
          double coordinate = get_double_from_string(unit, &execute_status);
          if (execute_status != NUMBER) {
            free_heap_string(unit);
            free(dots);
            return execute_status;
          }
          dots[real_dots_size++] = coordinate;
        }
        if ((execute_status = serialize_integer(real_dots_size, s_string)) != SUCCESS_FUNCTION_RETURN) {
          free_heap_string(unit);
          free(dots);
          return execute_status;
        }
        for (int i = 0; i < real_dots_size; ++i) {
          if ((execute_status = serialize_double(dots[i], STANDART_EVAL_PRECITION, s_string)) != SUCCESS_FUNCTION_RETURN) {
            free_heap_string(unit);
            free(dots);
            return execute_status;
          }
        }
        free(dots);
      } else {
        if ((execute_status = serialize_integer(2, s_string)) != SUCCESS_FUNCTION_RETURN) {
          free_heap_string(unit);
          return execute_status;
        }
        polynomial* pol = parse_polynomial(unit);
        if (pol == NULL) {
          free_heap_string(unit);
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        if ((execute_status = serialize_polynomial(s_string, pol)) != SUCCESS_FUNCTION_RETURN) {
          free_heap_string(unit);
          free_polynomial(pol);
          return execute_status;
        }
        free_polynomial(pol);
        if ((execute_status = read_string(FROM_STRING, request, is_comma, unit, &index)) != SUCCESS_FUNCTION_RETURN) {
          free_heap_string(unit);
          return execute_status;
        }
        pol = parse_polynomial(unit);
        if (pol == NULL) {
          free_heap_string(unit);
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        if ((execute_status = serialize_polynomial(s_string, pol)) != SUCCESS_FUNCTION_RETURN) {
          free_heap_string(unit);
          free_polynomial(pol);
          return execute_status;
        }
        free_polynomial(pol);
        if ((execute_status = serialize_integer(0, s_string)) != SUCCESS_FUNCTION_RETURN) {
          free_heap_string(unit);
          return execute_status;
        }
      }
    }
  }
  free_heap_string(unit);
  return SUCCESS_FUNCTION_RETURN;
}

struct dereq* deserialize_request(serialized_string* s_string, int* index) {
  struct dereq* de_request = (struct dereq*)calloc(1, sizeof(struct dereq));
  if (de_request == NULL) {
    return NULL;
  }
  de_request->instruction = deserialize_string(s_string, index);
  if (de_request->instruction == NULL) {
    free_request(de_request);
    return NULL;
  }
  de_request->amount = deserialize_integer(s_string, index);
  if ((!strcmp(de_request->instruction, "Print") || !strcmp(de_request->instruction, "Grad")) && de_request->amount == 0) {
    de_request->polynomials = NULL;
    de_request->dot = NULL;
    return de_request;
  }
  if (de_request->amount == 0) {
    de_request->polynomials = NULL;
  } else if (de_request->amount == 1) {
    de_request->polynomials = (polynomial**)malloc(sizeof(polynomial*));
    if (de_request->polynomials == NULL) {
      free_request(de_request);
      return NULL;
    }
    de_request->polynomials[0] = deserialize_polynomial(s_string, index);
    if (de_request->polynomials[0] == NULL) {
      free_request(de_request);
      return NULL;
    }
  } else {
    de_request->polynomials = (polynomial**)malloc(sizeof(polynomial*) * 2);
    if (de_request->polynomials == NULL) {
      free_request(de_request);
      return NULL;
    }
    de_request->polynomials[0] = deserialize_polynomial(s_string, index);
    if (de_request->polynomials[0] == NULL) {
      free_request(de_request);
      return NULL;
    }
    de_request->polynomials[1] = deserialize_polynomial(s_string, index);
    if (de_request->polynomials[1] == NULL) {
      free_request(de_request);
      return NULL;
    }
  }
  int dots_count = deserialize_integer(s_string, index);
  if (dots_count == 0) {
    de_request->dot = NULL;
  } else {
    de_request->dot = malloc(sizeof(*de_request->dot));
    if (de_request->dot == NULL) {
      free_request(de_request);
      return NULL;
    }
    de_request->dot->count = dots_count;
    de_request->dot->coordinates = (double*)malloc(sizeof(double) * dots_count);
    if (de_request->dot->coordinates == NULL) {
      free_request(de_request);
      return NULL;
    }
    for (int i = 0; i < dots_count; ++i) {
      de_request->dot->coordinates[i] = deserialize_double(s_string, STANDART_EVAL_PRECITION, index);
    }
  }
  return de_request;
}

void free_request(struct dereq* request) {
  free(request->instruction);
  for (int i = 0; i < request->amount; ++i) {
    free_polynomial(request->polynomials[i]);
  }
  free(request->polynomials);
  if (request->dot != NULL) {
    free(request->dot->coordinates);
    free(request->dot);
  }
  free(request);
}

#ifdef ___DEBUG___

void print_deserialized_request(struct dereq* request) {
  print_string(stdout, "instruction = %p\npolynomial amount = %d\nPolynomials:\n", request->instruction, request->amount);
  if (request->amount == 2) {
    print_polynomial(request->polynomials[0]);
    print_polynomial(request->polynomials[1]);
  } else if (request->amount == 1) {
    print_polynomial(request->polynomials[0]);
  } else {
    print_string(stdout, "NO POLYNOMIALS!\n");
  }
  if (request->dot == NULL) {
    print_string(stdout, "NO DOTS!\n");
  } else {
    print_string(stdout, "Dots count = %d\nCoordinates: ", request->dot->count);
    for (int i = 0; i < request->dot->count; ++i) {
      print_string(stdout, "%f ", request->dot->coordinates[i]);
    }
    print_string(stdout, "\n");
  }
}

#endif

void standart_monomial_clear_function(void* structure) {

}

polynomial* deserialize_polynomial(serialized_string* s_string, int* index) {
  polynomial* element = (polynomial*)malloc(sizeof(polynomial));
  if (element == NULL) {
    return NULL;
  }
  element->monomials = (list*)malloc(sizeof(list));
  if (element->monomials == NULL) {
    free(element);
    return NULL;
  }
  create_list(element->monomials);
  int monomials_size = deserialize_integer(s_string, index);
  for (int i = 0; i < monomials_size; ++i) {
    monomial* monomial_element = NULL;
    if ((monomial_element = deserialize_monomial(s_string, index)) == NULL) {
      list_free(element->monomials, standart_monomial_clear_function);
      free(element->monomials);
      free(element);
      free(monomial_element);
      return NULL;
    }
    if (list_push_back(element->monomials, monomial_element) != SUCCESS_FUNCTION_RETURN) {
      list_free(element->monomials, standart_monomial_clear_function);
      free(element->monomials);
      free(element);
      free(monomial_element);
      return NULL;
    }
  }
  element->free_term = deserialize_integer(s_string, index);
  return element;
}

int get_serialized_string_size(serialized_string* s_string) {
  return s_string->data_size;
}

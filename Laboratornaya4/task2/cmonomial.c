#include "cmonomial.h"

int monomial_comporator(void* lhs, void* rhs, int* execute_status) {
  monomial* l = (monomial*)lhs;
  monomial* r = (monomial*)rhs;
  if (l->data_size != r->data_size) {
    return 0;
  }
  for (int i = 0; i < l->data_size; ++i) {
    if (strcmp(l->unknowns[i]->name, r->unknowns[i]->name) || (l->unknowns[i]->power != r->unknowns[i]->power)) {
      return 0;
    }
  }
  return 1;
}

int standart_monomial_comporator(const pair** lhs, const pair** rhs) {
  int execute_status;
  string* lhs_str = heap_string((*lhs)->name);
  if (lhs_str == NULL) {
    return 0;
  }
  int result = string_compare("%p", standart_alphabet_comporator, &execute_status, lhs_str, (*rhs)->name);
  if (execute_status != SUCCESS_FUNCTION_RETURN) {
    free_heap_string(lhs_str);
    return 0;
  }
  free_heap_string(lhs_str);
  return result;
}

monomial* create_monomial() {
  monomial* mono = (monomial*)malloc(sizeof(monomial));
  if (mono == NULL) {
    return NULL;
  }
  mono->data_size = 0;
  mono->struct_size = STANDART_MONOMIAL_SIZE;
  mono->unknowns = (pair**)malloc(sizeof(pair*) * mono->struct_size);
  if (mono->unknowns == NULL) {
    free(mono);
    return NULL;
  }
  mono->coefficient = 0;
  return mono;
}

monomial* parse_monomial(string* monomial_element, int* is_free_term) {
  to_lower_case(monomial_element);
  int execute_status;
  if (string_is_integer(monomial_element)) {
    monomial* created_monomial = (monomial*)malloc(sizeof(monomial));
    if (created_monomial == NULL) {
      return NULL;
    }
    created_monomial->data_size = 0;
    created_monomial->struct_size = STANDART_MONOMIAL_SIZE;
    created_monomial->unknowns = (pair**)malloc(sizeof(pair*) * created_monomial->struct_size);
    if (created_monomial->unknowns == NULL) {
      free(created_monomial);
      return NULL;
    }
    created_monomial->coefficient = get_integer_from_string(monomial_element, &execute_status);
    if (execute_status != NUMBER) {
      free(created_monomial->unknowns);
      free(created_monomial);
      return NULL;
    }
    *is_free_term = 1;
    return created_monomial;
  }
  *is_free_term = 0;
  if (string_concat("%c", monomial_element, '_') != SUCCESS_FUNCTION_RETURN) {
    return NULL;
  }
  hash_table* client_table = create_hash_table(HASH_TABLE_SIZE);
  if (client_table == NULL) {
    return NULL;
  }
  string* coefficient_string = heap_string(EMPTY);
  if (coefficient_string == NULL) {
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
      hash_table_free(client_table + i, standart_hash_table_clear);
    }
    free(client_table);
    return NULL;
  }
  int i = 0;
  while (cin(sat(monomial_element, i), '0', '9', GG)) {
    if (string_concat("%c", coefficient_string, sat(monomial_element, i++)) != SUCCESS_FUNCTION_RETURN) {
      for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        hash_table_free(client_table + i, standart_hash_table_clear);
      }
      free(client_table);
      free_heap_string(coefficient_string);
      return NULL;
    }
  }
  monomial* created_monomial = (monomial*)malloc(sizeof(monomial));
  if (created_monomial == NULL) {
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
      hash_table_free(client_table + i, standart_hash_table_clear);
    }
    free(client_table);
    free_heap_string(coefficient_string);
    return NULL;
  }
  if (i == 0) {
    created_monomial->coefficient = 1;
  } else {
    created_monomial->coefficient = get_integer_from_string(coefficient_string, &execute_status);
    if (execute_status != NUMBER) {
      for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        hash_table_free(client_table + i, standart_hash_table_clear);
      }
      free(client_table);
      free(created_monomial);
      free_heap_string(coefficient_string);
      return NULL;
    }
  }
  free_heap_string(coefficient_string);
  char symbol;
  int flag = 0;
  string* variable_string = heap_string(EMPTY);
  if (variable_string == NULL) {
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
      hash_table_free(client_table + i, standart_hash_table_clear);
    }
    free(client_table);
    free(created_monomial);
    return NULL;
  }
  string* power_string = heap_string(EMPTY);
  if (power_string == NULL) {
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
      hash_table_free(client_table + i, standart_hash_table_clear);
    }
    free(client_table);
    free(created_monomial);
    free_heap_string(variable_string);
    return NULL;
  }
  created_monomial->data_size = 0;
  created_monomial->struct_size = STANDART_MONOMIAL_SIZE;
  created_monomial->unknowns = (pair**)malloc(sizeof(pair*) * created_monomial->struct_size);
  if (created_monomial->unknowns == NULL) {
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
      hash_table_free(client_table + i, standart_hash_table_clear);
    }
    free(client_table);
    free(created_monomial);
    free_heap_string(variable_string);
    free_heap_string(power_string);
    return NULL;
  }
  int first_symbol_flag = 1;
  int is_like_digit = 1;
  while (symbol = sat(monomial_element, i)) {
    if (symbol == '*') {
      ++i;
      continue;
    }
    if (symbol == '_') {
      int possible_power;
      if (is_string_empty(power_string)) {
        possible_power = 1;
        free_heap_string(power_string);
      } else {
        possible_power = get_integer_from_string(power_string, &execute_status);
        free_heap_string(power_string);
        if (execute_status != NUMBER) {
          for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
            hash_table_free(client_table + j, standart_hash_table_clear);
          }
          free(client_table);
          for (int j = 0; j < created_monomial->data_size; ++j) {
            free(created_monomial->unknowns[j]->name);
            free(created_monomial->unknowns[j]);
          }
          free(created_monomial->unknowns);
          free(created_monomial);
          free_heap_string(variable_string);
          return NULL;
        }
      }
      char* possible_name = get_trimmed_string(variable_string, &execute_status);
      free_heap_string(variable_string);
      int name_hash = get_hash(possible_name);
      pair rhs = { .name = possible_name, .power = -1 };
      pair* unknown = hash_table_get(client_table, name_hash, pair_comporator, &rhs);
      if (unknown != NULL) {
        unknown->power += possible_power;
        free(possible_name);
      } else if (possible_power != 0) {
        unknown = (pair*)malloc(sizeof(pair));
        if (unknown == NULL) {
          for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
            hash_table_free(client_table + j, standart_hash_table_clear);
          }
          free(client_table);
          for (int j = 0; j < created_monomial->data_size; ++j) {
            free(created_monomial->unknowns[j]->name);
            free(created_monomial->unknowns[j]);
          }
          free(created_monomial->unknowns);
          free(created_monomial);
          return NULL;
        }
        unknown->name = possible_name;
        unknown->power = possible_power;
        if (possible_power > 0) {
          is_like_digit = 0;
        }
        hash_table_put(client_table, name_hash, unknown);
        if (created_monomial->data_size == created_monomial->struct_size) {
          pair** possible_unknowns = (pair**)realloc(created_monomial->unknowns, sizeof(pair*) * 2 * created_monomial->struct_size);
          if (possible_unknowns == NULL) {
            for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
              hash_table_free(client_table + j, standart_hash_table_clear);
            }
            free(client_table);
            free(unknown);
            for (int j = 0; j < created_monomial->data_size; ++j) {
              free(created_monomial->unknowns[j]->name);
              free(created_monomial->unknowns[j]);
            }
            free(created_monomial->unknowns);
            free(created_monomial);
            return NULL;
          }
          created_monomial->struct_size *= 2;
          created_monomial->unknowns = possible_unknowns;
        }
        created_monomial->unknowns[created_monomial->data_size++] = unknown;
      }
      variable_string = heap_string(EMPTY);
      if (variable_string == NULL) {
        for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
          hash_table_free(client_table + j, standart_hash_table_clear);
        }
        free(client_table);
        for (int j = 0; j < created_monomial->data_size; ++j) {
          free(created_monomial->unknowns[j]->name);
          free(created_monomial->unknowns[j]);
        }
        free(created_monomial->unknowns);
        free(created_monomial);
        return NULL;
      }
      power_string = heap_string(EMPTY);
      if (power_string == NULL) {
        for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
          hash_table_free(client_table + j, standart_hash_table_clear);
        }
        free(client_table);
        for (int j = 0; j < created_monomial->data_size; ++j) {
          free(created_monomial->unknowns[j]->name);
          free(created_monomial->unknowns[j]);
        }
        free(created_monomial->unknowns);
        free(created_monomial);
        free_heap_string(variable_string);
        return NULL;
      }
      first_symbol_flag = 1;
      flag = 0;
      ++i;
      continue;
    }
    if (symbol == '^') {
      flag = 1;
      ++i;
      continue;
    }
    if (flag) {
      if (string_concat("%c", power_string, symbol) != SUCCESS_FUNCTION_RETURN) {
        for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
          hash_table_free(client_table + j, standart_hash_table_clear);
        }
        free(client_table);
        for (int j = 0; j < created_monomial->data_size; ++j) {
          free(created_monomial->unknowns[j]->name);
          free(created_monomial->unknowns[j]);
        }
        free(created_monomial->unknowns);
        free(created_monomial);
        free_heap_string(variable_string);
        free_heap_string(power_string);
        return NULL;
      }
    } else {
      if (string_concat("%c", variable_string, symbol) != SUCCESS_FUNCTION_RETURN || (first_symbol_flag && !cin(symbol, 'a', 'z', GG)) || (!cin(symbol, 'a', 'z', GG) && !cin(symbol, '0', '9', GG))) {
        for (int j = 0; j < HASH_TABLE_SIZE; ++j) {
          hash_table_free(client_table + j, standart_hash_table_clear);
        }
        free(client_table);
        for (int j = 0; j < created_monomial->data_size; ++j) {
          free(created_monomial->unknowns[j]->name);
          free(created_monomial->unknowns[j]);
        }
        free(created_monomial->unknowns);
        free(created_monomial);
        free_heap_string(variable_string);
        free_heap_string(power_string);
        return NULL;
      }
      first_symbol_flag = 0;
    }
    ++i;
  }
  free_heap_string(variable_string);
  free_heap_string(power_string);
  for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
    hash_table_free(client_table + i, standart_hash_table_clear);
  }
  free(client_table);
  qsort(created_monomial->unknowns, created_monomial->data_size, sizeof(pair*), standart_monomial_comporator);
  if (is_like_digit) {
    *is_free_term = 1;
  }
  return created_monomial;
}

void print_monomial(monomial* element, int new_line) {
  if (element->data_size == 0) {
    print_string(stdout, "%d", element->coefficient);
    if (new_line) {
      printf("\n");
    }
    return;
  }
  if (element->coefficient != 1 && element->coefficient != -1) {
    print_string(stdout, "%d", element->coefficient);
  } else if (element->coefficient == -1) {
    print_string(stdout, "-");
  }
  for (int i = 0; i < element->data_size; ++i) {
    if (element->unknowns[i]->power > 1) {
      printf("%s^%d", element->unknowns[i]->name, element->unknowns[i]->power);
    } else {
      printf("%s", element->unknowns[i]->name);
    }
  }
  new_line ? printf("\n") : NULL;
}

void free_monomial(monomial* element) {
  for (int i = 0; i < element->data_size; ++i) {
    free(element->unknowns[i]->name);
    free(element->unknowns[i]);
  }
  free(element->unknowns);
  free(element);
}

int standart_monomial_collector(int symbol, int index) {
  if (symbol == '+' || symbol == '-' || symbol =='\0') {
    return 1;
  }
  return 0;
}

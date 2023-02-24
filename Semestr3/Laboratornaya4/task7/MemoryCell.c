#include "MemoryCell.h"

MemoryCell** variables = NULL;
int variables_size = 0;

MemoryCell** get_variables() {
  return variables;
}

int get_variables_size() {
  return variables_size;
}

void clear_variables() {
  for (int i = 0; i < variables_size; ++i) {
    free_heap_string(variables[i]->name);
    free(variables[i]);
  }
  free(variables);
}

int get_variable_value(MemoryCell* variable) {
  return variable->value;
}

string* get_variable_name(MemoryCell* variable) {
  return variable->name;
}

void set_variable_value(MemoryCell* variable, int value) {
  variable->value = value;
}

MemoryCell* get_variable_or_null(string* name) {
  if (variables == NULL) {
    return NULL;
  }
  int left = 0;
  int right = variables_size - 1;
  while (left <= right) {
    int mid = (right - left) / 2 + left;
    int cmp_result = scompare_s(variables[mid]->name, name);
    if (cmp_result == FIRST_STRING) {
      right = mid - 1;
    } else if (cmp_result == SECOND_STRING) {
      left = mid + 1;
    } else {
      return variables[mid];
    }
   }
   return NULL;
}

int var_name_cmp(const MemoryCell** lhs, const MemoryCell** rhs) {
  return scompare_s((*lhs)->name, (*rhs)->name);
}

int add_variable(string* name, int value) {
  if (variables != NULL) {
    for (int i = 0; i < variables_size; ++i) {
      if (!scompare_s(variables[i], name)) {
        return WRONG_NUMBER_OR_VARIABLE;
      }
    }
  }
  if (!string_contains_only(name, standart_string_range)) {
    return ARGUMENTS_EXCEPTION;
  }
  MemoryCell* possible_variable = (MemoryCell*)malloc(sizeof(MemoryCell));
  if (possible_variable == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  possible_variable->name = heap_string(get_raw_string(name));
  if (possible_variable->name == NULL) {
    free(possible_variable);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  possible_variable->value = value;
  MemoryCell** possible_variables = (MemoryCell**)realloc(variables, sizeof(MemoryCell*) * (variables_size + 1));
  if (possible_variables == NULL) {
    free_heap_string(possible_variable->name);
    free(possible_variable);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  variables = possible_variables;
  variables[variables_size++] = possible_variable;
  qsort(variables, variables_size, sizeof(MemoryCell*), var_name_cmp);
  return SUCCESS_FUNCTION_RETURN;
}

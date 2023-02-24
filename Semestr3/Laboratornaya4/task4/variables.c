#include "variables.h"

int* arrays[26] = {NULL};
int arrays_size[26] = {0};

void get_variable_or_null(char name, int** array, int* execute_status) {
  if (!cin(name, 'A', 'Z', GG)) {
    *execute_status = WRONG_VARIABLE_NAME;
    return NULL;
  }
  *array = arrays[name - 'A'];
  if (*array == NULL) {
    *execute_status = UNINITIALIZED_VARIABLE;
  } else {
    *execute_status = SUCCESS_FUNCTION_RETURN;
  }
}

int update_variable(char name, int* variable, int size) {
  if (!cin(name, 'A', 'Z', GG)) {
    return WRONG_VARIABLE_NAME;
  }
  arrays[name - 'A'] = variable;
  arrays_size[name - 'A'] = size;
}

int get_array_size(char name, int* execute_status) {
  if (!cin(name, 'A', 'Z', GG)) {
    return WRONG_VARIABLE_NAME;
  }
  return arrays_size[name - 'A'];
}

void kill_variables() {
  for (int i = 0; i < 26; ++i) {
    if (arrays[i] != NULL) {
      free(arrays[i]);
    }
  }
}

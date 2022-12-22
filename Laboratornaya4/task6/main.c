#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "cexpression.h"
#include "stack.h"

#define TREE_BUILD_EXCEPTION 17

int read_till_new_line(int symbol, int index) {
  return (symbol == '\n' || symbol == EOF) ? 1 : 0;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    print_string(stdout, "There must be 1 file given as an argument!\n");
    return ARGUMENTS_EXCEPTION;
  }
  int execute_status;
  string* expression = heap_string(EMPTY);
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    print_string(stdout, "Seems like wrong file was given!\n");
    return FILE_INTERRACT_EXCEPTION;
  }
  if (read_string(file, expression, isspace_s) == MEMORY_ALLOCATE_EXCEPTION) {
    fclose(file);
    free_heap_string(expression);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  fclose(file);
  to_lower_case(expression);
  int linker[26];
  for (int i = 0; i < 26; ++i) {
    linker[i] = -1;
  }
  if (validate_expression(expression) == 0) {
    print_string(stdout, "Unexpected expression! Check input.\n");
    return 0;
  }
  tree* expression_tree = get_expression_tree(expression, &execute_status, linker);
  if (expression_tree == NULL || execute_status != SUCCESS_FUNCTION_RETURN) {
    free_heap_string(expression);
    return TREE_BUILD_EXCEPTION;
  }
  int amount_of_variables = 0;
  for (int i = 0; i < 26; ++i) {
    if (linker[i] == 1) {
      print_string(stdout,"%c ", i + 'a');
      linker[i] = amount_of_variables;
      ++amount_of_variables;
    }
  }
  print_string(stdout, "%s\n", expression);
  print_table_by_expression_tree(expression_tree, amount_of_variables, linker);
  free_tree(expression_tree);
  free_heap_string(expression);
  return 0;
}

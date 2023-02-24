#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "cconstants.h"
#include "tree_builder.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  int execute_status;
  string* str = heap_string(EMPTY);
  if (str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    free_heap_string(str);
    return FILE_INTERRACT_EXCEPTION;
  }
  FILE* out_file = fopen("output", "w");
  if (out_file == NULL) {
    free_heap_string(str);
    fclose(file);
    return FILE_INTERRACT_EXCEPTION;
  }
  while ((execute_status = read_string(file, str, isspace)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    ctree* tree = (ctree*)malloc(sizeof(ctree));
    if (tree == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    create_tree(tree);
    if ((execute_status = build_tree(str, tree)) != SUCCESS_FUNCTION_RETURN) {
      free_tree(tree);
      return execute_status;
    }
    print_tree(tree, 0, out_file);
    print_string(out_file, "=========\n");
    free_tree(tree);
  }
  free_heap_string(str);
  fclose(file);
  fclose(out_file);
  return 0;
}

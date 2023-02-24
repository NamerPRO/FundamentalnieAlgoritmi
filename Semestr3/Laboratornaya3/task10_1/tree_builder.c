#include "tree_builder.h"

int read_sep(int symbol) {
  return (symbol == ',' || symbol == '(') ? 1 : 0;
}

void create_tree(ctree* t) {
  t->children = NULL;
  t->children_count = 0;
  t->value = NULL;
}

void free_tree(ctree* t) {
  children_free_tree(t);
  free(t->children);
  free_heap_string(t->value);
  free(t);
}

void children_free_tree(ctree* t) {
  if (t->children != NULL) {
    for (int i = 0; i < t->children_count; ++i) {
      children_free_tree(t->children[i]);
      free(t->children[i]->children);
      free_heap_string(t->children[i]->value);
      free(t->children[i]);
    }
  }
}

void print_tree(ctree* t, int h, FILE* stream) {
  if (t != NULL) {
    for (int i = 0; i < h; ++i) {
      print_string(stream, " ");
    }
    print_string(stream, "%s\n", t->value);
    for (int i = 0; i < t->children_count; ++i) {
      print_tree(t->children[i], h + 1, stream);
    }
  }
}

int insert_tree_node_in_stack(stack* st, string* str) {
  ctree* elem = (ctree*)malloc(sizeof(ctree));
  if (elem == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  elem->children = NULL;
  elem->children_count = 0;
  elem->value = str;
  stack_push(elem, st);
}

int fill_array_from_stack(stack* st, ctree*** cchildren, int* i) {
  ctree* elem = stack_top(st);
  stack_pop(st, stack_clear_function);
  if (elem == NULL) {
    return SUCCESS_FUNCTION_RETURN;
  }
  if (fill_array_from_stack(st, cchildren, i) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  ctree** possible_cchildren = (ctree**)realloc(*cchildren, sizeof(ctree*) * (*i + 1));
  if (possible_cchildren == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  *cchildren = possible_cchildren;
  (*cchildren)[(*i)++] = elem;
  return SUCCESS_FUNCTION_RETURN;
}

int build_tree(string* pattern, ctree* tree) {
  int execute_status; int position = 0;
  char symbol; stack st;
  stack_create(&st);
  string* str = heap_string(EMPTY);
  if (str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (unsigned long i = 0; symbol = get_raw_string(pattern)[i]; ++i) {
    if (isalnum(symbol)) {
      if ((execute_status = string_concat("%c", str, symbol)) != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
    } else if (symbol == ',' || symbol == '(') {
      if (get_string_size(str)) {
        if ((execute_status = insert_tree_node_in_stack(&st, str)) != SUCCESS_FUNCTION_RETURN) {
          return execute_status;
        }
        if (symbol == '(') {
          stack_push(OPEN_BRACKET, &st);
        }
        str = heap_string(EMPTY);
        if (str == NULL) {
          return MEMORY_ALLOCATE_EXCEPTION;
        }
      }
    } else if (symbol == ')') {
      if (get_string_size(str)) {
        if ((execute_status = insert_tree_node_in_stack(&st, str)) != SUCCESS_FUNCTION_RETURN) {
          return execute_status;
        }
        str = heap_string(EMPTY);
        if (str == NULL) {
          return MEMORY_ALLOCATE_EXCEPTION;
        }
      }
      ctree** children = NULL;
      int count = 0;
      if ((execute_status = fill_array_from_stack(&st, &children, &count)) != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
      ctree* elem = stack_top(&st);
      elem->children = children;
      elem->children_count = count;
      stack_pop(&st, stack_clear_function);
      stack_push(elem, &st);
    }
  }
  if (stack_empty(&st)) {
    if ((execute_status = insert_tree_node_in_stack(&st, str)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  } else {
    free_heap_string(str);
  }
  *tree = *stack_top(&st);
  stack_pop(&st, stack_clear_function);
  return SUCCESS_FUNCTION_RETURN;
}

#include "cexpression.h"

int operator_cmp(int symbol, int index) {
  return isalpha(symbol) || isdigit(symbol) || symbol == '\0';
}

int validate_brackets(string* expression) {
  stack st;
  create_stack(&st);
  for (int i = 0; sat(expression, i); ++i) {
    if (sat(expression, i) == '(') {
      stack_push(&st, '(');
      continue;
    }
    if (sat(expression, i) == ')') {
      if (!stack_empty(&st)) {
        stack_pop(&st, standart_clear_function);
      } else {
        return 0;
      }
      continue;
    }
  }
  if (!stack_empty(&st)) {
    stack_clear(&st, standart_clear_function);
    return 0;
  }
  return 1;
}

int validate_expression(string* expression) {
  if (!validate_brackets(expression)) {
    return 0;
  }
  int is_variable_phase = 1;
  char* expr = get_raw_string(expression);
  sized_string* sstr = heap_sized_string(EMPTY);
  if (sstr == NULL) {
    return -1;
  }
  int cnt = 0;
  int i = 0;
  for (; expr[i]; ++i) {
    if (expr[i] == '(' && (i == 0 || expr[i - 1] == '~')) {
      continue;
    }
    if (expr[i] == '(') {
      if (expr[i - 1] == '(') {
        continue;
      }
      if (is_variable_phase) {
        free_heap_sized_string(sstr);
        return 0;
      }
      if (cnt == 0 || cnt > 2) {
        free_heap_sized_string(sstr);
        return 0;
      }
      if (get_priority(sstr) == -1) {
        free_heap_sized_string(sstr);
        return 0;
      }
      sized_string_reset(sstr);
      cnt = 0;
      is_variable_phase = 1;
      continue;
    }
    if (expr[i] == ')') {
      if (expr[i - 1] == ')') {
        continue;
      }
      if (expr[i - 1] == '(' || is_variable_phase == 0) {
        free_heap_sized_string(sstr);
        return 0;
      }
      if (cnt == 0) {
        free_heap_sized_string(sstr);
        return 0;
      }
      cnt = 0;
      is_variable_phase = 0;
      continue;
    }
    if (is_variable_phase) {
      if (cnt == 0 && expr[i] == '~') {
        continue;
      } else if (cnt > 0 && expr[i] == '~') {
        free_heap_sized_string(sstr);
        return 0;
      }
      if (isalpha(expr[i]) || expr[i] == '0' || expr[i] == '1') {
        ++cnt;
        if (cnt > 1) {
          free_heap_sized_string(sstr);
          return 0;
        }
      } else {
        if (cnt == 0) {
          free_heap_sized_string(sstr);
          return 0;
        }
        cnt = 0;
        is_variable_phase = 0;
        --i;
      }
    } else {
      if (expr[i] == '~' && (isalpha(expr[i + 1]) || expr[i + 1] == '0' || expr[i + 1] == '1' || expr[i + 1] == '(' || expr[i + 1] == '~')
          && (!isalpha(expr[i - 1]) && expr[i - 1] != '0' && expr[i - 1] != '1')) {
        continue;
      } else if (expr[i] == '~') {
        free_heap_sized_string(sstr);
        return 0;
      }
      if (!isalpha(expr[i]) && expr[i] != '0' && expr[i] != '1') {
        ++cnt;
        if (cnt > 2) {
          free_heap_sized_string(sstr);
          return 0;
        }
        sized_string_concat("%c", sstr, expr[i]);
      } else {
        if (cnt == 0 || get_priority(sstr) == -1) {
          free_heap_sized_string(sstr);
          return 0;
        }
        cnt = 0;
        is_variable_phase = 1;
        sized_string_reset(sstr);
        --i;
      }
    }
  }
  if (!isalpha(expr[i - 1]) && expr[i - 1] != '0' && expr[i - 1] != '1' && expr[i - 1] != ')') {
    free_heap_sized_string(sstr);
    return 0;
  }
  free_heap_sized_string(sstr);
  return 1;
}

int get_priority(string* symb) {
  char* symbol = get_raw_string(symb);
  if (symbol[0] == '~' && symbol[1] == '\0') {
    return 7;
  }
  if (symbol[0] == '&' && symbol[1] == '\0') {
    return 6;
  }
  if (symbol[0] == '|' && symbol[1] == '\0' || (symbol[0] == '<' && symbol[1] == '>')) {
    return 5;
  }
  if (symbol[0] == '!' && symbol[1] == '\0') {
    return 4;
  }
  if (symbol[0] == '?' && symbol[1] == '\0') {
    return 3;
  }
  if ((symbol[0] =='-' && symbol[1] == '>') || (symbol[0] == '+' && symbol[1] == '>')) {
    return 2;
  }
  if (symbol[0] == '=' && symbol[1] == '\0') {
    return 1;
  }
  return -1;
  // if (symbol[0] == '~') {
  //   return 3;
  // }
  // if (symbol[0] == '?' || symbol[0] == '!' ||
  //     symbol[0] == '&' || (symbol[0] == '+' && symbol[1] == '>')) {
  //   return 2;
  // }
  // if (symbol[0] == '|' || (symbol[0] =='-' && symbol[1] == '>') ||
  //     (symbol[0] == '<' && symbol[1] == '>') || symbol[0] == '=') {
  //   return 1;
  // }
}

void print_tree(tree_node* node, int h) {
  if (node != NULL) {
    print_tree(node->left, h + 1);
    printf("%*s%s\n", h, "", node->value->str);
    print_tree(node->right, h + 1);
  }
}

void free_tree(tree_node* node) {
  if (node != NULL) {
    free_tree(node->left);
    free_tree(node->right);
    free_heap_sized_string(node->value);
    free(node);
  }
}

void standart_tree_clear(void* structure) {
  free_tree(structure);
}

int is_inverse(int symbol) {
  return (symbol == '~') ? 1 : 0;
}

void print_table_by_expression_tree(tree_node* tr, int n, int* linker) {
  char* permutation = (char*)malloc(sizeof(char) * n);
  if (permutation == NULL) {
    return NULL;
  }
  for (int i = 0; i < (1 << n); ++i) {
    int perm_as_int = i;
    for (int j = 0; j < n; ++j) {
      permutation[j] = perm_as_int & 1;
      perm_as_int >>= 1;
    }
    for (int j = n - 1; j >= 0; --j) {
      printf("%d ", permutation[j]);
    }
    printf("%d\n", evaluate_by_expression_tree(tr, permutation, n, linker));
  }
  free(permutation);
}

char evaluate_by_expression_tree(tree_node* node, char* permutation, int n, int* linker) {
  if (node != NULL) {
    char* symbol = get_raw_string(node->value);
    if (symbol[0] == '~') {
      return !evaluate_by_expression_tree(node->left, permutation, n, linker);
    }
    if (symbol[0] == '?') {
      return !(evaluate_by_expression_tree(node->left, permutation, n, linker) | evaluate_by_expression_tree(node->right, permutation, n, linker));
    }
    if (symbol[0] == '!') {
      return !(evaluate_by_expression_tree(node->left, permutation, n, linker) & evaluate_by_expression_tree(node->right, permutation, n, linker));
    }
    if (symbol[0] == '&') {
      return evaluate_by_expression_tree(node->left, permutation, n, linker) & evaluate_by_expression_tree(node->right, permutation, n, linker);
    }
    if (symbol[0] == '+' && symbol[1] == '>') {
      return evaluate_by_expression_tree(node->left, permutation, n, linker) & !evaluate_by_expression_tree(node->right, permutation, n, linker);
    }
    if (symbol[0] == '|') {
      return evaluate_by_expression_tree(node->left, permutation, n, linker) | evaluate_by_expression_tree(node->right, permutation, n, linker);
    }
    if (symbol[0] =='-' && symbol[1] == '>') {
      return !evaluate_by_expression_tree(node->left, permutation, n, linker) | evaluate_by_expression_tree(node->right, permutation, n, linker);
    }
    if (symbol[0] == '<' && symbol[1] == '>') {
      return evaluate_by_expression_tree(node->left, permutation, n, linker) ^ evaluate_by_expression_tree(node->right, permutation, n, linker);
    }
    if (symbol[0] == '=') {
      return evaluate_by_expression_tree(node->left, permutation, n, linker) == evaluate_by_expression_tree(node->right, permutation, n, linker);
    }
    if (symbol[0] == '0' || symbol[0] == '1') {
      return symbol[0] - '0';
    }
    return permutation[n - linker[symbol[0] - 'a'] - 1];
  }
}

//учесть, что оператор стоит на своем месте
tree_node* get_expression_tree(string* expression, int* execute_status, int* linker) {
  if (is_string_empty(expression)) {
    return NULL;
  }
  stack st1;
  create_stack(&st1);
  stack st2;
  create_stack(&st2);
  int flag = 0;
  unsigned long int i = 0;
  do {
    sized_string* operator = heap_sized_string(EMPTY);
    if (operator == NULL) {
      stack_clear(&st1, standart_tree_clear);
      stack_clear(&st2, standart_tree_clear);
      return NULL;
    }
    char symbol = sat(expression, i);
    while (symbol != '(' && symbol != ')' && !cin(symbol, 'a', 'z', GG)
            && symbol != '1' && symbol != '0' && symbol != '~') {
      if (sized_string_concat("%c", operator, symbol) == MEMORY_ALLOCATE_EXCEPTION) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        free_heap_sized_string(operator);
        return NULL;
      }
      symbol = sat(expression, ++i);
    }
    if (is_sized_string_empty(operator)) {
      ++i;
      if (sized_string_concat("%c", operator, symbol) == MEMORY_ALLOCATE_EXCEPTION) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        free_heap_sized_string(operator);
        return NULL;
      }
    }
    if (cin(__char(operator), 'a', 'z', GG) || __char(operator) == '1' || __char(operator) == '0') {
      if (cin(__char(operator), 'a', 'z', GG)) {
        if (linker[__char(operator) - 'a'] == -1) {
          linker[__char(operator) - 'a'] = 1;
        }
      }
      tree_node* node = create_node(operator);
      if (node == NULL) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        free_heap_sized_string(operator);
        free(node);
        return NULL;
      }
      if (stack_push(&st1, node) == MEMORY_ALLOCATE_EXCEPTION) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        free_heap_sized_string(operator);
        free(node);
        return NULL;
      }
      continue;
    }
    if (stack_empty(&st2) || __char(operator) == '(' || (stack_top(&st2, execute_status) == NULL && __char(operator) != ')')) {
      if (__char(operator) == '(') {
        free_heap_sized_string(operator);
        if (stack_push(&st2, NULL) == MEMORY_ALLOCATE_EXCEPTION) {
          stack_clear(&st1, standart_tree_clear);
          stack_clear(&st2, standart_tree_clear);
          return NULL;
        }
        continue;
      }
      tree_node* node = create_node(operator);
      if (node == NULL) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        free_heap_sized_string(operator);
        free(node);
        return NULL;
      }
      if (stack_push(&st2, node) == MEMORY_ALLOCATE_EXCEPTION) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        free_heap_sized_string(operator);
        free(node);
        return NULL;
      }
      continue;
    }
    if (__char(operator) == ')') {
      free_heap_sized_string(operator);
      while (stack_top(&st2, execute_status) != NULL) {
        tree_node* top_operation = stack_top(&st2, execute_status);
        stack_pop(&st2, standart_clear_function);
        if (__char(top_operation->value) != '~') {
          top_operation->right = stack_top(&st1, execute_status);
          stack_pop(&st1, standart_clear_function);
        }
        top_operation->left = stack_top(&st1, execute_status);
        stack_pop(&st1, standart_clear_function);
        if (stack_push(&st1, top_operation) == MEMORY_ALLOCATE_EXCEPTION) {
          stack_clear(&st1, standart_tree_clear);
          stack_clear(&st2, standart_tree_clear);
          free_tree(top_operation);
          return NULL;
        }
      }
      stack_pop(&st2, standart_clear_function);
      continue;
    }
    if (!is_inverse(__char(operator))) {
      while (!stack_empty(&st2) && stack_top(&st2, execute_status) != NULL &&
            get_priority(operator) <= get_priority(stack_top(&st2, execute_status)->value)) {
          tree_node* top_operation = stack_top(&st2, execute_status);
          stack_pop(&st2, standart_clear_function);
          if (__char(top_operation->value) != '~') {
            top_operation->right = stack_top(&st1, execute_status);
            stack_pop(&st1, standart_clear_function);
          }
          top_operation->left = stack_top(&st1, execute_status);
          stack_pop(&st1, standart_clear_function);
          if (stack_push(&st1, top_operation) == MEMORY_ALLOCATE_EXCEPTION) {
            stack_clear(&st1, standart_tree_clear);
            stack_clear(&st2, standart_tree_clear);
            free_tree(top_operation);
            return NULL;
          }
      }
      tree_node* add_node = create_node(operator);
      if (add_node == NULL) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        return NULL;
      }
      if (stack_push(&st2, add_node) == MEMORY_ALLOCATE_EXCEPTION) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        free_heap_sized_string(operator);
        free(add_node);
        return NULL;
      }
    } else {
      while (!stack_empty(&st2) && stack_top(&st2, execute_status) != NULL &&
            get_priority(operator) < get_priority(stack_top(&st2, execute_status)->value)) {
          tree_node* top_operation = stack_top(&st2, execute_status);
          stack_pop(&st2, standart_clear_function);
          if (__char(top_operation->value) != '~') {
            top_operation->right = stack_top(&st1, execute_status);
            stack_pop(&st1, standart_clear_function);
          }
          top_operation->left = stack_top(&st1, execute_status);
          stack_pop(&st1, standart_clear_function);
          if (stack_push(&st1, top_operation) == MEMORY_ALLOCATE_EXCEPTION) {
            stack_clear(&st1, standart_tree_clear);
            stack_clear(&st2, standart_tree_clear);
            free_tree(top_operation);
            return NULL;
          }
      }
      tree_node* add_node = create_node(operator);
      if (add_node == NULL) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        return NULL;
      }
      if (stack_push(&st2, add_node) == MEMORY_ALLOCATE_EXCEPTION) {
        stack_clear(&st1, standart_tree_clear);
        stack_clear(&st2, standart_tree_clear);
        free_heap_sized_string(operator);
        free(add_node);
        return NULL;
      }
    }
  } while (sat(expression, i));
  while (!stack_empty(&st2)) {
    tree_node* top_operation = stack_top(&st2, execute_status);
    stack_pop(&st2, standart_clear_function);
    if (__char(top_operation->value) != '~') {
      top_operation->right = stack_top(&st1, execute_status);
      stack_pop(&st1, standart_clear_function);
    }
    top_operation->left = stack_top(&st1, execute_status);
    stack_pop(&st1, standart_clear_function);
    if (stack_push(&st1, top_operation) == MEMORY_ALLOCATE_EXCEPTION) {
      stack_clear(&st1, standart_tree_clear);
      stack_clear(&st2, standart_tree_clear);
      free_tree(top_operation);
      return NULL;
    }
  }
  *execute_status = SUCCESS_FUNCTION_RETURN;
  tree_node* result = stack_top(&st1, execute_status);
  stack_pop(&st1, standart_clear_function);
  return result;
}

tree_node* create_node(sized_string* value) {
  tree_node* node = (tree_node*)malloc(sizeof(tree_node));
  if (node == NULL) {
    return NULL;
  }
  node->left = NULL;
  node->right = NULL;
  node->value = value;
}

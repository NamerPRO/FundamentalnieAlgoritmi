#include "cexpression.h"

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

int is_operation(char symbol) {
  switch (symbol) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '^':
    case '(':
    case ')':
      return 1;
    default:
      return 0;
  }
}

int operand_cmp(int symbol, int index) {
  return is_operation(symbol) || symbol == '\0';
}

int get_priority(int symbol) {
  switch (symbol) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
    case '%':
      return 2;
    case '^':
      return 3;
  }
}

int is_left_associative(int symbol) {
  switch (symbol) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '^':
      return 1;
  }
  return 0;
}

int evaluate_expression(string* postfix_expression, int* execute_status) {
  unsigned long int i = 0;
  stack_int st;
  create_stack_int(&st);
  string* lexeme = heap_string(EMPTY);
  if (lexeme == NULL) {
    return -1;
  }
  do {
    if (read_string(FROM_STRING, postfix_expression, isspace_s, lexeme, &i) == MEMORY_ALLOCATE_EXCEPTION) {
      free_heap_sized_string(lexeme);
      stack_int_clear(&st, standart_clear_function);
      return NULL;
    }
    if (sat(lexeme, 1) || !is_operation(_char(lexeme))) {
      int number = get_integer_from_string(lexeme, execute_status);
      if (*execute_status != NUMBER) {
        free_heap_sized_string(lexeme);
        stack_int_clear(&st, standart_clear_function);
        return -1;
      }
      if (stack_int_push(&st, number) == MEMORY_ALLOCATE_EXCEPTION) {
        free_heap_sized_string(lexeme);
        stack_int_clear(&st, standart_clear_function);
        return -1;
      }
    } else {
      int first, second;
      first = stack_int_top(&st, execute_status);
      stack_int_pop(&st, standart_clear_function);
      second = stack_int_top(&st, execute_status);
      stack_int_pop(&st, standart_clear_function);
      switch (_char(lexeme)) {
        case '+':
          if (stack_int_push(&st, first + second) == MEMORY_ALLOCATE_EXCEPTION) {
            free_heap_sized_string(lexeme);
            stack_int_clear(&st, standart_clear_function);
            return -1;
          }
          break;
        case '-':
          if (stack_int_push(&st, second - first) == MEMORY_ALLOCATE_EXCEPTION) {
            free_heap_sized_string(lexeme);
            stack_int_clear(&st, standart_clear_function);
            return -1;
          }
          break;
        case '*':
          if (stack_int_push(&st, first * second) == MEMORY_ALLOCATE_EXCEPTION) {
            free_heap_sized_string(lexeme);
            stack_int_clear(&st, standart_clear_function);
            return -1;
          }
          break;
        case '/':
          if (first == 0) {
            *execute_status = DEVISION_BY_ZERO;
            free_heap_sized_string(lexeme);
            stack_int_clear(&st, standart_clear_function);
            return -1;
          }
          if (stack_int_push(&st, second / first) == MEMORY_ALLOCATE_EXCEPTION) {
            free_heap_sized_string(lexeme);
            stack_int_clear(&st, standart_clear_function);
            return -1;
          }
          break;
        case '%':
          if (stack_int_push(&st, ((second % first) + first) % first) == MEMORY_ALLOCATE_EXCEPTION) {
            free_heap_sized_string(lexeme);
            stack_int_clear(&st, standart_clear_function);
            return -1;
          }
          break;
        case '^':
          if (stack_int_push(&st, (int)pow(second, first)) == MEMORY_ALLOCATE_EXCEPTION) {
            free_heap_sized_string(lexeme);
            stack_int_clear(&st, standart_clear_function);
            return -1;
          }
          break;
      }
    }
  } while (sat(postfix_expression, i));
  free_heap_sized_string(lexeme);
  int result = stack_int_top(&st, execute_status);
  stack_int_pop(&st, standart_clear_function);
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return result;
}

string* get_postfix_form(string* expression, int* execute_status) {
  if (is_string_empty(expression)) {
    return NULL;
  }
  stack st;
  create_stack(&st);
  sized_string* postfix_sized_string = heap_sized_string(EMPTY);
  if (postfix_sized_string == NULL) {
    return NULL;
  }
  unsigned long int i = 0;
  string* operand = heap_string(EMPTY);
  if (operand == NULL) {
    free_heap_sized_string(postfix_sized_string);
    return NULL;
  }
  int negative_flag = 0;
  do {
    if (sat(expression, i) == '-') {
      negative_flag = 1;
      ++i;
      continue;
    }
    if (read_string(FROM_STRING, expression, operand_cmp, operand, &i) == MEMORY_ALLOCATE_EXCEPTION) {
      free_heap_sized_string(postfix_sized_string);
      free_heap_string(operand);
      return NULL;
    }
    if (!is_string_empty(operand)) {
      if (negative_flag) {
        if (sized_string_concat("%c", postfix_sized_string, '-') == MEMORY_ALLOCATE_EXCEPTION) {
          free_heap_sized_string(postfix_sized_string);
          free_heap_string(operand);
          return NULL;
        }
      }
      if (sized_string_concat("%s%c", postfix_sized_string, operand, ' ') == MEMORY_ALLOCATE_EXCEPTION) {
        free_heap_sized_string(postfix_sized_string);
        free_heap_string(operand);
        return NULL;
      }
    }
    negative_flag = 0;
    char operation = sat(expression, i - 1);
    if (!operation) {
      break;
    }
    if (stack_empty(&st) || operation == '(' || (stack_top(&st, execute_status) == '(' && operation != ')')) {
      if (stack_push(&st, operation) == MEMORY_ALLOCATE_EXCEPTION) {
        free_heap_sized_string(postfix_sized_string);
        free_heap_string(operand);
        return NULL;
      }
      continue;
    }
    if (operation == ')') {
      while (stack_top(&st, execute_status) != '(') {
        char top_operation = stack_top(&st, execute_status);
        stack_pop(&st, standart_clear_function);
        if (sized_string_concat("%c%c", postfix_sized_string, top_operation, ' ') == MEMORY_ALLOCATE_EXCEPTION) {
          free_heap_sized_string(postfix_sized_string);
          free_heap_string(operand);
          return NULL;
        }
      }
      stack_pop(&st, standart_clear_function);
      continue;
    }
    if (is_left_associative(operation)) {
      while (!stack_empty(&st) && get_priority(operation) <= get_priority(stack_top(&st, execute_status))
            && stack_top(&st, execute_status) != '(') {
          char top_operation = stack_top(&st, execute_status);
          stack_pop(&st, standart_clear_function);
          if (sized_string_concat("%c%c", postfix_sized_string, top_operation, ' ') == MEMORY_ALLOCATE_EXCEPTION) {
            free_heap_sized_string(postfix_sized_string);
            free_heap_string(operand);
            return NULL;
          }
      }
      if (stack_push(&st, operation) == MEMORY_ALLOCATE_EXCEPTION) {
        free_heap_sized_string(postfix_sized_string);
        free_heap_string(operand);
        return NULL;
      }
      continue;
    } else {
      while (!stack_empty(&st) && get_priority(operation) < get_priority(stack_top(&st, execute_status))
            && stack_top(&st, execute_status) != '(') {
          char top_operation = stack_top(&st, execute_status);
          stack_pop(&st, standart_clear_function);
          if (sized_string_concat("%c%c", postfix_sized_string, top_operation, ' ') == MEMORY_ALLOCATE_EXCEPTION) {
            free_heap_sized_string(postfix_sized_string);
            free_heap_string(operand);
            return NULL;
          }
      }
      if (stack_push(&st, operation) == MEMORY_ALLOCATE_EXCEPTION) {
        free_heap_sized_string(postfix_sized_string);
        free_heap_string(operand);
        return NULL;
      }
      continue;
    }
  } while (1);
  free_heap_string(operand);
  while (!stack_empty(&st)) {
    char top_operation = stack_top(&st, execute_status);
    stack_pop(&st, standart_clear_function);
    if (sized_string_concat("%c%c", postfix_sized_string, top_operation, ' ') == MEMORY_ALLOCATE_EXCEPTION) {
      free_heap_sized_string(postfix_sized_string);
      free_heap_string(operand);
      return NULL;
    }
  }
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return clear_size(postfix_sized_string);
}

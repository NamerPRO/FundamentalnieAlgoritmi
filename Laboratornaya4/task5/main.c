#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "cexpression.h"

int read_till_new_line(int symbol, int index) {
  return (symbol == '\n' || symbol == EOF) ? 1 : 0;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    print_string(stdout, "There must be at least 1 file with expressions!\n");
    return ARGUMENTS_EXCEPTION;
  }
  int execute_status;
  string* expression = heap_string(EMPTY);
  for (int i = 1; i < argc; ++i) {
    FILE* file = fopen(argv[i], "r");
    if (file == NULL) {
      free_heap_string(expression);
      return FILE_INTERRACT_EXCEPTION;
    }
    FILE* log_file = NULL;
    print_string(stdout, "===================\nReading data from file %p...\n", argv[i]);
    unsigned long int expression_index = 0;
    while ((execute_status = read_string(file, expression, read_till_new_line)) != EOF) {
      if (execute_status != SUCCESS_FUNCTION_RETURN) {
        free_heap_string(expression);
        fclose(file);
        return execute_status;
      }
      if (validate_brackets(expression)) {
        string* postfix_string = get_postfix_form(expression, &execute_status);
        int result = evaluate_expression(postfix_string, &execute_status);
        if (execute_status == DEVISION_BY_ZERO) {
          if (log_file == NULL) {
            strcat(argv[i], "_error_log");
            log_file = fopen(argv[i], "w");
            if (log_file == NULL) {
              free_heap_string(expression);
              fclose(file);
              return FILE_INTERRACT_EXCEPTION;
            }
          }
          print_string(log_file, "expression=%s position=%d reason=devision by zero\n", expression, expression_index);
        } else if (execute_status == SUCCESS_FUNCTION_RETURN) {
          print_string(stdout, "-----------------\nInitial expression: %s\nParsed expression: %s\nExpression value: %d\n-----------------\n", expression, postfix_string, result);
        } else {
          if (log_file == NULL) {
            strcat(argv[i], "_error_log");
            log_file = fopen(argv[i], "w");
            if (log_file == NULL) {
              free_heap_string(expression);
              fclose(file);
              return FILE_INTERRACT_EXCEPTION;
            }
          }
          print_string(log_file, "expression=%s position=%d reason=unevaluateable expression\n", expression, expression_index);
        }
        free_heap_string(postfix_string);
      } else {
        if (log_file == NULL) {
          strcat(argv[i], "_error_log");
          log_file = fopen(argv[i], "w");
          if (log_file == NULL) {
            free_heap_string(expression);
            fclose(file);
            return FILE_INTERRACT_EXCEPTION;
          }
        }
        print_string(log_file, "expression=%s position=%d reason=brakets disbalance\n", expression, expression_index);
      }
      ++expression_index;
    }
    if (log_file != NULL) {
      fclose(log_file);
    }
    fclose(file);
    print_string(stdout, "===================\n");
  }
  free_heap_string(expression);
  return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "MemoryCell.h"
#include "cnumber.h"

int command_separator(int symbol, int index) {
  return symbol == ';';
}

int command_splitter(int symbol, int index) {
  return !symbol;
}

int get_first(int symbol, int index) {
  return symbol == '=' || symbol == ' ';
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    print_string(stdout, "Failed to open a file!\n");
    return FILE_INTERRACT_EXCEPTION;
  }
  string* unit = heap_string(EMPTY);
  if (unit == NULL) {
    print_string(stdout, "Failed due to lack of memory.\n");
    fclose(file);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  int execute_status;
  while ((execute_status = read_string(file, unit, command_separator)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      free_heap_string(unit);
      fclose(file);
      clear_variables();
      return execute_status;
    }
    getc(file);
    string* first = heap_string(EMPTY);
    if (first == NULL) {
      free_heap_string(unit);
      fclose(file);
      clear_variables();
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    unsigned long int ii = 0;
    if ((execute_status = read_string(FROM_STRING, unit, get_first, first, &ii)) != SUCCESS_FUNCTION_RETURN) {
      free_heap_string(unit);
      free_heap_string(first);
      clear_variables();
      fclose(file);
      return execute_status;
    }
    int cmp_res = !scompare(first, "print", &execute_status);
    if (execute_status == MEMORY_ALLOCATE_EXCEPTION) {
      free_heap_string(unit);
      free_heap_string(first);
      clear_variables();
      fclose(file);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if (cmp_res) {
      char* c_cmd = get_raw_string(unit);
      int is_space = 0;
      unsigned long int i = 0;
      for (; c_cmd[i]; ++i) {
        if (c_cmd[i] == ' ') {
          is_space = 1;
          ++i;
          break;
        }
      }
      if (!is_space) {
        MemoryCell** variables = get_variables();
        for (int j = 0; j < get_variables_size(); ++j) {
          print_string(stdout, "%s = %d\n", get_variable_name(variables[j]), get_variable_value(variables[j]));
        }
        free_heap_string(first);
        continue;
      }
      string* var_name = heap_string(EMPTY);
      if (var_name == NULL) {
        free_heap_string(unit);
        free_heap_string(first);
        fclose(file);
        clear_variables();
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      if ((execute_status = read_string(FROM_STRING, unit, command_splitter, var_name, &i)) != SUCCESS_FUNCTION_RETURN) {
        free_heap_string(unit);
        free_heap_string(first);
        free_heap_string(var_name);
        fclose(file);
        clear_variables();
        return execute_status;
      }
      MemoryCell* variable = get_variable_or_null(var_name);
      if (variable == NULL) {
        free_heap_string(unit);
        free_heap_string(first);
        free_heap_string(var_name);
        fclose(file);
        clear_variables();
        print_string(stdout, "Variable was not declared!\n");
        return WRONG_NUMBER_OR_VARIABLE;
      }
      print_string(stdout, "%d (%s)\n", get_variable_value(variable), var_name);
      free_heap_string(first);
      free_heap_string(var_name);
      continue;
    }
    char* c_cmd = get_raw_string(unit);
    unsigned long int i = 0;
    for (; c_cmd[i]; ++i) {
      if (c_cmd[i] == '=') {
        ++i;
        break;
      }
    }
    string* expr = heap_string(EMPTY);
    if (expr == NULL) {
      free_heap_string(unit);
      free_heap_string(first);
      fclose(file);
      clear_variables();
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if ((execute_status = read_string(FROM_STRING, unit, command_splitter, expr, &i)) != SUCCESS_FUNCTION_RETURN) {
      free_heap_string(unit);
      free_heap_string(first);
      free_heap_string(expr);
      fclose(file);
      clear_variables();
      return execute_status;
    }
    char* c_expr = get_raw_string(expr);
    sized_string* before = heap_sized_string(EMPTY);
    if (before == NULL) {
      free_heap_string(unit);
      free_heap_string(first);
      free_heap_string(expr);
      fclose(file);
      clear_variables();
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    sized_string* after = heap_sized_string(EMPTY);
    if (after == NULL) {
      free_heap_string(unit);
      free_heap_string(first);
      free_heap_string(expr);
      free_heap_sized_string(before);
      clear_variables();
      fclose(file);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    int beforeOperation = 1;
    char operation = 0;
    for (int j = 0; c_expr[j]; ++j) {
      if (c_expr[j] == '+' || c_expr[j] == '-' || c_expr[j] == '*'
           || c_expr[j] == '/' || c_expr[j] == '%') {
        operation = c_expr[j];
        beforeOperation = 0;
        continue;
      }
      if (beforeOperation) {
        sized_string_concat("%c", before, c_expr[j]);
      } else {
        sized_string_concat("%c", after, c_expr[j]);
      }
    }
    string* before_s = clear_size(before);
    MemoryCell* var = get_variable_or_null(first);
    if (var == NULL) {
      if ((execute_status = add_variable(first, 0)) != SUCCESS_FUNCTION_RETURN) {
        free_heap_string(unit);
        free_heap_string(first);
        free_heap_string(expr);
        free_heap_string(before_s);
        free_heap_sized_string(after);
        clear_variables();
        fclose(file);
        if (execute_status == ARGUMENTS_EXCEPTION) {
          print_string(stdout, "Variable you try to add has wrong name!\n");
        }
        return execute_status;
      }
      var = get_variable_or_null(first);
    }
    if (beforeOperation) {
      if (string_is_integer(before_s)) {
        int value = get_integer_from_string(before_s, &execute_status);
        if (execute_status != NUMBER) {
          free_heap_string(unit);
          free_heap_string(first);
          free_heap_string(expr);
          free_heap_string(before_s);
          free_heap_sized_string(after);
          fclose(file);
          clear_variables();
          print_string(stdout, "Too big/small number passed.\n");
          return WRONG_NUMBER_OR_VARIABLE;
        }
        set_variable_value(var, value);
      } else {
        MemoryCell* var2 = get_variable_or_null(before_s);
        if (var2 == NULL) {
          print_string(stdout, "Undeclared variable '%s' found! Stopping interpriter.\n", before_s);
          free_heap_string(unit);
          free_heap_string(first);
          free_heap_string(expr);
          free_heap_string(before_s);
          free_heap_sized_string(after);
          fclose(file);
          clear_variables();
          return WRONG_NUMBER_OR_VARIABLE;
        }
        set_variable_value(var, get_variable_value(var2));
      }
      free_heap_string(first);
      free_heap_string(expr);
      free_heap_string(before_s);
      free_heap_sized_string(after);
      continue;
    }
    string* after_s = clear_size(after);
    int before_i, after_i;
    if (string_is_integer(before_s)) {
      before_i = get_integer_from_string(before_s, &execute_status);
      if (execute_status != NUMBER) {
        free_heap_string(unit);
        free_heap_string(first);
        free_heap_string(expr);
        free_heap_string(before_s);
        free_heap_string(after_s);
        fclose(file);
        clear_variables();
        print_string(stdout, "Too big/small number passed.\n");
        return WRONG_NUMBER_OR_VARIABLE;
      }
      if (string_is_integer(after_s)) {
        after_i = get_integer_from_string(after_s, &execute_status);
        if (execute_status != NUMBER) {
          free_heap_string(unit);
          free_heap_string(first);
          free_heap_string(expr);
          free_heap_string(before_s);
          free_heap_string(after_s);
          fclose(file);
          clear_variables();
          print_string(stdout, "Too big/small number passed.\n");
          return WRONG_NUMBER_OR_VARIABLE;
        }
      } else {
        MemoryCell* var3 = get_variable_or_null(after_s);
        if (var3 == NULL) {
          print_string(stdout, "Undeclared variable '%s' found! Stopping interpriter.\n", after_s);
          free_heap_string(unit);
          free_heap_string(first);
          free_heap_string(expr);
          free_heap_string(before_s);
          free_heap_string(after_s);
          fclose(file);
          clear_variables();
          return WRONG_NUMBER_OR_VARIABLE;
        }
        after_i = get_variable_value(var3);
      }
    } else {
      MemoryCell* var3 = get_variable_or_null(before_s);
      if (var3 == NULL) {
        print_string(stdout, "Undeclared variable '%s' found! Stopping interpriter.\n", before_s);
        free_heap_string(unit);
        free_heap_string(first);
        free_heap_string(expr);
        free_heap_string(before_s);
        free_heap_string(after_s);
        fclose(file);
        clear_variables();
        return WRONG_NUMBER_OR_VARIABLE;
      }
      before_i = get_variable_value(var3);
      if (string_is_integer(after_s)) {
        after_i = get_integer_from_string(after_s, &execute_status);
        if (execute_status != NUMBER) {
          free_heap_string(unit);
          free_heap_string(first);
          free_heap_string(expr);
          free_heap_string(before_s);
          free_heap_string(after_s);
          fclose(file);
          clear_variables();
          print_string(stdout, "Too big/small number passed.\n");
          return WRONG_NUMBER_OR_VARIABLE;
        }
      } else {
        MemoryCell* var4 = get_variable_or_null(after_s);
        if (var4 == NULL) {
          print_string(stdout, "Undeclared variable '%s' found! Stopping interpriter.\n", after_s);
          free_heap_string(unit);
          free_heap_string(first);
          free_heap_string(expr);
          free_heap_string(before_s);
          free_heap_string(after_s);
          fclose(file);
          clear_variables();
          return WRONG_NUMBER_OR_VARIABLE;
        }
        after_i = get_variable_value(var4);
      }
    }
    int res;
    switch (operation) {
      case '+':
        res = before_i + after_i;
        break;
      case '-':
        res = before_i - after_i;
        break;
      case '*':
        res = before_i * after_i;
        break;
      case '/':
        if (after_i == 0) {
          print_string(stdout, "Devision by zero not allowed!\n");
          set_variable_value(var, res);
          free_heap_string(first);
          free_heap_string(expr);
          free_heap_string(before_s);
          free_heap_string(after_s);
          fclose(file);
          free_heap_string(unit);
          clear_variables();
          return DEVISION_BY_ZERO;
        }
        res = before_i / after_i;
        break;
      case '%':
        if (after_i == 0) {
          print_string(stdout, "Devision by zero not allowed!\n");
          set_variable_value(var, res);
          free_heap_string(first);
          free_heap_string(expr);
          free_heap_string(before_s);
          free_heap_string(after_s);
          fclose(file);
          free_heap_string(unit);
          clear_variables();
          return DEVISION_BY_ZERO;
        }
        res = (after_i + (before_i % after_i)) % after_i;
        break;
    }
    set_variable_value(var, res);
    free_heap_string(first);
    free_heap_string(expr);
    free_heap_string(before_s);
    free_heap_string(after_s);
  }
  fclose(file);
  free_heap_string(unit);
  clear_variables();
  return 0;
}

#include <stdio.h>
#include <ctype.h>

#include "clist.h"
#include "cconstants.h"
#include "cstring.h"
#include "cnumber.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  int execute_status;
  string* num_str = heap_string(argv[1]);
  if (num_str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  unsigned long int n = get_integer_from_string(num_str, &execute_status);
  free_heap_string(num_str);
  if (execute_status != NUMBER) {
    return execute_status;
  }
  string* str = heap_string(EMPTY);
  if (str == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  FILE* file_log = fopen("log", "a");
  if (file_log == NULL) {
    free_heap_string(str);
    return FILE_INTERRACT_EXCEPTION;
  }
  list lst;
  create_list(&lst);
  unsigned long int count = 0;
  while ((execute_status = read_string(stdin, str, isspace)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      list_clear(&lst, standart_string_clear_function);
      free_heap_string(str);
      fclose(file_log);
      return execute_status;
    }
    if (!string_compare("%p", standart_string_comporator, str, "back")) {
      if (list_empty(&lst) || !count) {
        print_string(stdout, "Cannot cancel action as stack is empty!\n");
        continue;
      }
      if ((execute_status = list_pop_back(&lst, standart_clear_function)) != SUCCESS_FUNCTION_RETURN) {
        list_clear(&lst, standart_string_clear_function);
        free_heap_string(str);
        fclose(file_log);
        return execute_status;
      }
      --count;
      continue;
    }
    if ((execute_status = list_push_back(&lst, str)) != SUCCESS_FUNCTION_RETURN) {
      list_clear(&lst, standart_string_clear_function);
      free_heap_string(str);
      fclose(file_log);
      return execute_status;
    }
    if (count < n / 2) {
      ++count;
    }
    if (list_size(&lst) == n) {
      while (!list_empty(&lst)) {
        print_string(file_log, "%s\n", list_element_at(&lst, 0, &execute_status));
        if (execute_status != SUCCESS_FUNCTION_RETURN) {
          return execute_status;
        }
        if ((execute_status = list_pop_front(&lst, standart_string_clear_function)) != SUCCESS_FUNCTION_RETURN) {
          list_clear(&lst, standart_string_clear_function);
          free_heap_string(str);
          fclose(file_log);
          return execute_status;
        }
      }
      count = 0;
    }
    str = heap_string(EMPTY);
    if (str == NULL) {
      list_clear(&lst, standart_string_clear_function);
      free_heap_string(str);
      fclose(file_log);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
  }
  free_heap_string(str);
  while (!list_empty(&lst)) {
    print_string(file_log, "%s\n", list_element_at(&lst, 0, &execute_status));
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      list_clear(&lst, standart_string_clear_function);
      free_heap_string(str);
      fclose(file_log);
      return execute_status;
    }
    if ((execute_status = list_pop_front(&lst, standart_string_clear_function)) != SUCCESS_FUNCTION_RETURN) {
      list_clear(&lst, standart_string_clear_function);
      free_heap_string(str);
      fclose(file_log);
      return execute_status;
    }
  }
  fclose(file_log);
  return 0;
}

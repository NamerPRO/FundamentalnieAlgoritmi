#include <stdio.h>

#include "cconstants.h"
#include "list_priority_queue.h"
#include "cstring.h"
#include "cconstants.h"
#include "cnumber.h"

typedef enum data_state {
  PRIORITY,
  TASK
} data_state;

int quote_cmp(int symbol) {
  return (symbol == '\'') ? 1 : 0;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    print_string(stdout, "At least one file with data must be given as a parameter!\n");
    return ARGUMENTS_EXCEPTION;
  }
  int execute_status;
  int files = argc - 1;
  FILE** file_arr = (FILE**)malloc(sizeof(FILE*) * files);
  int i = 0;
  for (; i < files; ++i) {
    file_arr[i] = fopen(argv[i + 1], "r");
    if (file_arr[i] == NULL) {
      for (int j = 0; j < i; ++j) {
        fclose(file_arr[j]);
      }
      free(file_arr);
      return FILE_INTERRACT_EXCEPTION;
    }
  }
  int count_closed_files = 0;
  list_queue lq; create_list_queue(&lq);
  int possible_symbol; char symbol = 0;
  char* prior_pattern = "prior"; char* task_pattern = "task";
  i = 0; int flag;
  data_state state = PRIORITY;
  for (int j = 0; count_closed_files < files; j = (j + 1) % files) {
    if (file_arr[j] == NULL) {
      continue;
    }
    if ((possible_symbol = getc(file_arr[j])) != EOF) {
      T* t = (T*)malloc(sizeof(T));
      flag = 1;
      symbol = (char)possible_symbol;
      while (flag) {
        switch (state) {
          case PRIORITY:
            if (symbol != prior_pattern[i]) {
              for (int k = 0; k < files; ++k) {
                if (file_arr[k] != NULL) {
                  fclose(file_arr[k]);
                }
              }
              free(file_arr);
              list_queue_destroy(&lq);
              free(t);
              return FILE_INTERRACT_EXCEPTION;
            }
            if (i == 4) {
              if (getc(file_arr[j]) != '=') {
                for (int k = 0; k < files; ++k) {
                  if (file_arr[k] != NULL) {
                    fclose(file_arr[k]);
                  }
                }
                free(file_arr);
                list_queue_destroy(&lq);
                free(t);
                return FILE_INTERRACT_EXCEPTION;
              }
              char digit = 0;
              int number = 0;
              while ((digit = getc(file_arr[j])) != ' ') {
                if (digit < '0' || digit > '9') {
                  for (int k = 0; k < files; ++k) {
                    if (file_arr[k] != NULL) {
                      fclose(file_arr[k]);
                    }
                  }
                  free(file_arr);
                  list_queue_destroy(&lq);
                  free(t);
                  return FILE_INTERRACT_EXCEPTION;
                }
                number = number * 10 + digit - '0';
                if (number > 10) {
                  for (int k = 0; k < files; ++k) {
                    if (file_arr[k] != NULL) {
                      fclose(file_arr[k]);
                    }
                  }
                  free(file_arr);
                  list_queue_destroy(&lq);
                  free(t);
                  return FILE_INTERRACT_EXCEPTION;
                }
              }
              if (digit != ' ') {
                for (int k = 0; k < files; ++k) {
                  if (file_arr[k] != NULL) {
                    fclose(file_arr[k]);
                  }
                }
                free(file_arr);
                list_queue_destroy(&lq);
                free(t);
                return FILE_INTERRACT_EXCEPTION;
              }
              t->priority = number;
              state = TASK;
              i = 0;
            } else {
              ++i;
            }
            break;
          case TASK:
            if (symbol != task_pattern[i]) {
              for (int k = 0; k < files; ++k) {
                if (file_arr[k] != NULL) {
                  fclose(file_arr[k]);
                }
              }
              free(file_arr);
              list_queue_destroy(&lq);
              free(t);
              return FILE_INTERRACT_EXCEPTION;
            }
            if (i == 3) {
              if (getc(file_arr[j]) != '=') {
                for (int k = 0; k < files; ++k) {
                  if (file_arr[k] != NULL) {
                    fclose(file_arr[k]);
                  }
                }
                free(file_arr);
                list_queue_destroy(&lq);
                free(t);
                return FILE_INTERRACT_EXCEPTION;
              }
              if (getc(file_arr[j]) != '\'') {
                for (int k = 0; k < files; ++k) {
                  if (file_arr[k] != NULL) {
                    fclose(file_arr[k]);
                  }
                }
                free(file_arr);
                list_queue_destroy(&lq);
                free(t);
                return FILE_INTERRACT_EXCEPTION;
              }
              string* s = (string*)malloc(sizeof(string));
              if (s == NULL) {
                for (int k = 0; k < files; ++k) {
                  if (file_arr[k] != NULL) {
                    fclose(file_arr[k]);
                  }
                }
                free(file_arr);
                list_queue_destroy(&lq);
                free(t);
                return MEMORY_ALLOCATE_EXCEPTION;
              }
              if ((execute_status = create_empty_string(s)) != SUCCESS_FUNCTION_RETURN) {
                for (int k = 0; k < files; ++k) {
                  if (file_arr[k] != NULL) {
                    fclose(file_arr[k]);
                  }
                }
                free(file_arr);
                list_queue_destroy(&lq);
                free(t);
                free(s);
                return execute_status;
              }
              if ((execute_status = read_string(file_arr[j], s, quote_cmp)) != SUCCESS_FUNCTION_RETURN) {
                for (int k = 0; k < files; ++k) {
                  if (file_arr[k] != NULL) {
                    fclose(file_arr[k]);
                  }
                }
                free(file_arr);
                list_queue_destroy(&lq);
                free(t);
                free_string(s);
                free(s);
                return execute_status;
              }
              t->text = s;
              i = 0;
              state = PRIORITY;
              flag = 0;
            } else {
              ++i;
            }
            break;
        }
        symbol = getc(file_arr[j]);
      }
      list_queue_insert(&lq, t, standart_priority_queue_comporator);
    } else {
      fclose(file_arr[j]);
      file_arr[j] = NULL;
      ++count_closed_files;
    }
  }
  free(file_arr);
  FILE* file = fopen("save_file", "w");
  if (file == NULL) {
    list_queue_destroy(&lq);
    return FILE_INTERRACT_EXCEPTION;
  }
  while (!list_queue_empty(&lq)) {
    LIST_QUEUE_T* lt = list_queue_get_maximum(&lq);
    print_string(file, "prior=%d task='%s'\n", lt->priority, lt->text);
    list_queue_extract_max(&lq, standart_list_queue_free);
  }
  fclose(file);
  return 0;
}

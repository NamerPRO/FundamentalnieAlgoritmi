#include <stdio.h>
#include <ctype.h>

#include "cnumber.h"
#include "cstring.h"
#include "cconstants.h"
#include "clist.h"
#include "villager.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    return FILE_INTERRACT_EXCEPTION;
  }
  list lst;
  create_list(&lst);
  read_villager_arr_from_file(file, &lst, 0);
  fclose(file);
  string action;
  int execute_status;
  if ((execute_status = create_empty_string(&action)) != SUCCESS_FUNCTION_RETURN) {
    list_clear(&lst, standart_villager_clear);
    return execute_status;
  }
  print_string(stdout, "=============\nsearch -> search villager by surname\nadd -> add villager to list\nremove -> remove villager by surname\nexport -> transfer data to file\n=============\n");
  while ((execute_status = read_string(stdin, &action, isspace)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      free_string(&action);
      list_clear(&lst, standart_villager_clear);
      return execute_status;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "search")) {
      list_node* lst_node = lst.head;
      string entered_surname;
      if ((execute_status = create_empty_string(&entered_surname)) != SUCCESS_FUNCTION_RETURN) {
        free_string(&action);
        list_clear(&lst, standart_villager_clear);
        return execute_status;
      }
      print_string(stdout, "Enter surname to search by:\n");
      if ((execute_status = read_string(stdin, &entered_surname, isspace)) != SUCCESS_FUNCTION_RETURN) {
        free_string(&action);
        free_string(&entered_surname);
        list_clear(&lst, standart_villager_clear);
        return execute_status;
      }
      int flag = 0;
      while (lst_node != NULL) {
        if (!string_compare("%s", standart_string_comporator, lst_node->data->surname, &entered_surname)) {
          flag = 1;
          print_string(stdout, "%s %s %s %s %p %f\n", lst_node->data->surname, lst_node->data->name, lst_node->data->middle_name, lst_node->data->date, lst_node->data->g == MALE ? "M" : "F", lst_node->data->salary);
          break;
        }
        lst_node = lst_node->next;
      }
      if (!flag) {
        print_string(stdout, "No villager found by given surname!\n");
      }
      free_string(&entered_surname);
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "add")) {
      print_string(stdout, "Enter surname, name, middle name, date, gender, salary via dot:\n");
      read_villager_arr_from_file(stdin, &lst, 1);
      print_string(stdout, "Villager was successfully added!\n");
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "remove")) {
      list_node* lst_node = lst.head;
      string entered_surname;
      if ((execute_status = create_empty_string(&entered_surname)) != SUCCESS_FUNCTION_RETURN) {
        free_string(&action);
        list_clear(&lst, standart_villager_clear);
        return execute_status;
      }
      print_string(stdout, "Enter surname to remove by:\n");
      if ((execute_status = read_string(stdin, &entered_surname, isspace)) != SUCCESS_FUNCTION_RETURN) {
        free_string(&action);
        free_string(&entered_surname);
        list_clear(&lst, standart_villager_clear);
        return execute_status;
      }
      int flag = 0;
      if ((execute_status = list_pop_by(&lst, &entered_surname, standart_villager_clear)) != SUCCESS_FUNCTION_RETURN) {
        if (execute_status == WRONG_INPUT_EXCEPTION) {
          flag = 1;
        } else {
          free_string(&action);
          free_string(&entered_surname);
          list_clear(&lst, standart_villager_clear);
          return execute_status;
        }
      }
      free_string(&entered_surname);
      print_string(stdout, flag ? "No such villager! Cannot remove nothing.\n" : "Villager successfully removed!\n");
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "export")) {
      file = fopen("villager_export", "w");
      if (file == NULL) {
        list_clear(&lst, standart_villager_clear);
        return FILE_INTERRACT_EXCEPTION;
      }
      list_node* lst_node = lst.head;
      while (lst_node != NULL) {
        print_string(file, "%s %s %s %s %p %f\n", lst_node->data->surname, lst_node->data->name, lst_node->data->middle_name, lst_node->data->date, lst_node->data->g == MALE ? "M" : "F", lst_node->data->salary);
        lst_node = lst_node->next;
      }
      fclose(file);
      print_string(stdout, "Data successfully exported to \"villager_export\" file!\n");
      continue;
    }
    print_string(stdout, "Unknown commad given! Use mentioned above.\n");
  }
  free_string(&action);
  if ((execute_status = list_clear(&lst, standart_villager_clear)) != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  return 0;
}

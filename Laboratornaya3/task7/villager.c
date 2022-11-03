#include "villager.h"

int standart_villager_insert_comporator(LIST_TYPE lhs, LIST_TYPE rhs) {
  return standart_date_comporator(lhs->date, rhs->date);
}

void standart_villager_clear(LIST_TYPE elem) {
  free_string(elem->surname);
  free(elem->surname);
  free_string(elem->name);
  free(elem->name);
  free_string(elem->middle_name);
  free(elem->middle_name);
  free_string(elem->date);
  free(elem->date);
}

int read_villager_arr_from_file(FILE* file, list* villager_list, int exit_type) {
  int execute_status;
  string** str_arr = (string**)malloc(sizeof(string*) * 6);
  while ((execute_status = file_read_n_until(file, str_arr, 6, standart_string_end, NEW_STRING)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      free_string_array(&str_arr, 4);
      fclose(file);
      return execute_status;
    }
    villager* vil = (villager*)malloc(sizeof(villager));
    if (!string_contains_only(str_arr[0], standart_string_range)) {
      free_string(str_arr[0]);
      free(str_arr[0]);
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free_string(str_arr[4]);
      free(str_arr[4]);
      free_string(str_arr[5]);
      free(str_arr[5]);
      free(vil);
      continue;
    }
    vil->surname = str_arr[0];
    if (!string_contains_only(str_arr[1], standart_string_range)) {
      free_string(str_arr[0]);
      free(str_arr[0]);
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free_string(str_arr[4]);
      free(str_arr[4]);
      free_string(str_arr[5]);
      free(str_arr[5]);
      free(vil);
      continue;
    }
    vil->name = str_arr[1];
    if (!string_contains_only(str_arr[2], standart_string_range)) {
      free_string(str_arr[0]);
      free(str_arr[0]);
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free_string(str_arr[4]);
      free(str_arr[4]);
      free_string(str_arr[5]);
      free(str_arr[5]);
      free(vil);
      continue;
    }
    vil->middle_name = str_arr[2];
    if (!validate_date(str_arr[3], NULL)) {
      free_string(str_arr[0]);
      free(str_arr[0]);
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free_string(str_arr[4]);
      free(str_arr[4]);
      free_string(str_arr[5]);
      free(str_arr[5]);
      free(vil);
      continue;
    }
    vil->date = str_arr[3];
    if (!string_compare("%p", standart_string_comporator, str_arr[4], "M")) {
      vil->g = MALE;
    } else if (!string_compare("%p", standart_string_comporator, str_arr[4], "F")) {
      vil->g = FEMALE;
    } else {
      free_string(str_arr[0]);
      free(str_arr[0]);
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free_string(str_arr[4]);
      free(str_arr[4]);
      free_string(str_arr[5]);
      free(str_arr[5]);
      free(vil);
      continue;
    }
    free_string(str_arr[4]);
    free(str_arr[4]);
    vil->salary = get_double_from_string(str_arr[5], &execute_status);
    if (execute_status != NUMBER) {
      free_string(str_arr[0]);
      free(str_arr[0]);
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free_string(str_arr[5]);
      free(str_arr[5]);
      free(vil);
      continue;
    }
    if (vil->salary < -STANDART_EPSIOLON) {
      free_string(str_arr[0]);
      free(str_arr[0]);
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free_string(str_arr[5]);
      free(str_arr[5]);
      free(vil);
      continue;
    }
    free_string(str_arr[5]);
    free(str_arr[5]);
    list_insert(villager_list, vil, standart_villager_insert_comporator);
    if (exit_type) {
      break;
    }
  }
  free(str_arr);
  return SUCCESS_FUNCTION_RETURN;
}

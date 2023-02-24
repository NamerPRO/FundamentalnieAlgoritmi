#include "employee.h"

int read_employee_arr_from_file(FILE* file, employee*** emp_array, int* emp_array_size) {
  int execute_status;
  employee_array employee_arr;
  if (create_employee_array(&employee_arr) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  string** str_arr = (string**)malloc(sizeof(string*) * 4);
  while ((execute_status = file_read_n_until(file, str_arr, 4, standart_string_end, NEW_STRING)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      free_string_array(&str_arr, 4);
      free_employee_array(employee_arr.array, employee_arr.real_size);
      fclose(file);
      return execute_status;
    }
    employee* emp = (employee*)malloc(sizeof(employee));
    emp->id = get_integer_from_string(str_arr[0], &execute_status);
    if (execute_status != NUMBER) {
      free_string(str_arr[0]);
      free(str_arr[0]);
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free(emp);
      continue;
    }
    free_string(str_arr[0]);
    free(str_arr[0]);
    if (!string_contains_only(str_arr[1], standart_string_range)) {
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free(emp);
      continue;
    }
    emp->name = str_arr[1];
    if (!string_contains_only(str_arr[2], standart_string_range)) {
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free(emp);
      continue;
    }
    emp->surname = str_arr[2];
    emp->salary = get_double_from_string(str_arr[3], &execute_status);
    if (execute_status != NUMBER) {
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free_string(str_arr[3]);
      free(str_arr[3]);
      free(emp);
      continue;
    }
    free_string(str_arr[3]);
    free(str_arr[3]);
    if (insert_employee_in_array(&employee_arr, emp) == MEMORY_ALLOCATE_EXCEPTION) {
      free_string(str_arr[1]);
      free(str_arr[1]);
      free_string(str_arr[2]);
      free(str_arr[2]);
      free(emp);
      free_employee_array(employee_arr.array, employee_arr.real_size);
      fclose(file);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
  }
  free(*str_arr);
  free(str_arr);
  *emp_array = get_employee_array(&employee_arr);
  *emp_array_size = get_employee_array_size(&employee_arr);
  return SUCCESS_FUNCTION_RETURN;
}

void free_employee_array(employee** emp_arr, unsigned long int size) {
  for (unsigned long int i = 0; i < size; ++i) {
    free_employee(emp_arr[i]);
    free(emp_arr[i]);
  }
  free(emp_arr);
}

void free_employee(employee* emp) {
  free_string(emp->name);
  free(emp->name);
  free_string(emp->surname);
  free(emp->surname);
}

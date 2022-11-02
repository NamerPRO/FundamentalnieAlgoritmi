#include <stdio.h>

#include "cfibo.h"
#include "clist.h"
#include "cfile.h"
#include "carray.h"
#include "cstring.h"
#include "cnumber.h"
#include "employee.h"

typedef enum FLAG_TYPE {
  A,
  D,
  UNKNOWN
} FLAG_TYPE;

FLAG_TYPE get_flag_type(char* str) {
  if (!strcmp(str, "-a") || !strcmp(str, "/a")) {
    return A;
  }
  if (!strcmp(str, "-d") || !strcmp(str, "/d")) {
    return D;
  }
  return UNKNOWN;
}

int salary_min_cmp(FIBO_T lhs, FIBO_T rhs) {
  return (lhs->salary < rhs->salary) ? 1 : 0;
}

int salary_max_cmp(FIBO_T lhs, FIBO_T rhs) {
  return (rhs->salary < lhs->salary) ? 1 : 0;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    return ARGUMENTS_EXCEPTION;
  }
  FLAG_TYPE flag = get_flag_type(argv[2]);
  int (*cmp)(FIBO_T lhs, FIBO_T rhs);
  switch (flag) {
    case A:
      cmp = salary_min_cmp;
      break;
    case D:
      cmp = salary_max_cmp;
      break;
    default:
      return UNKNOWN_FLAG_EXCEPTION;
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    return FILE_INTERRACT_EXCEPTION;
  }
  employee** emp_array;
  int emp_array_size;
  read_employee_arr_from_file(file, &emp_array, &emp_array_size);
  fclose(file);
  file = fopen(argv[3], "w");
  if (file == NULL) {
    return FILE_INTERRACT_EXCEPTION;
  }
  fibo fib;
  create_fibo(&fib);
  for (int i = 0; i < emp_array_size; ++i) {
    fibo_insert(&fib, emp_array[i], cmp);
  }
  for (int i = 0; i < emp_array_size; ++i) {
    employee* emp = fibo_get_minimum(&fib);
    print_string(file, "%d %s %s %f\n", emp->id, emp->name, emp->surname, emp->salary);
    fibo_remove_minimum(&fib, cmp);
  }
  fclose(file);
  free_employee_array(emp_array, emp_array_size);
  return 0;
}

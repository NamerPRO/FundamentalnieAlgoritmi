#ifndef ___CARRAY___
#define ___CARRAY___

#include <stdlib.h>

#include "cconstants.h"
#include "employee_struct.h"
#include "employee.h"

#define STANDART_ARRAY_SIZE 10

typedef struct employee_array {
  employee** array;
  unsigned long int total_size;
  unsigned long int real_size;
} employee_array;

employee** get_employee_array(employee_array* array);
unsigned long int get_employee_array_size(employee_array* array);
int create_employee_array(employee_array* array);
int insert_employee_in_array(employee_array* array, employee* element);
// void free_employee_array(employee_array* array);

#endif

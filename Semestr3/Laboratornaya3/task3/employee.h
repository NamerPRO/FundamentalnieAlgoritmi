#ifndef ___EMPLOYEE___
#define ___EMPLOYEE___

#include "cstring.h"
#include "carray.h"
#include "employee_struct.h"
#include "cfile.h"
#include "cnumber.h"

int read_employee_arr_from_file(FILE* file, employee*** emp_array, int* emp_array_size);
void free_employee_array(employee** emp_arr, unsigned long int size);
void free_employee(employee* emp);

#endif

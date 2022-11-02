#ifndef ___EMPLOYEE_STRUCT___
#define ___EMPLOYEE_STRUCT___

#include "cstring.h"

typedef struct employee {
  unsigned long int id;
  string* name;
  string* surname;
  double salary;
} employee;

#endif

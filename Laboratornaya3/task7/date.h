#ifndef ___DATE___
#define ___DATE___

#include "cstring.h"
#include "cnumber.h"

typedef struct date {
  int day, month, year;
} date;

int validate_date(string* str_date, date* dt);
int standart_date_comporator(string* str1, string* str2);

#endif

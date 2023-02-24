#ifndef ___DATE___
#define ___DATE___

#include "cstring.h"
#include "cinteger.h"

typedef struct date {
  int day, month, year;
  int hours, minutes, seconds;
} date;

int get_date(string* str_date, date* dt);
int standart_date_comporator(string* str1, string* str2);

#endif

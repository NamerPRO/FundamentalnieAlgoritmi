#ifndef ___CARRAY___
#define ___CARRAY___

#include <stdlib.h>

#include "cconstants.h"
#include "mail_struct.h"

#define STANDART_ARRAY_SIZE 10

typedef struct mail_array {
  mail** array;
  unsigned long int total_size;
  unsigned long int real_size;
} mail_array;

mail** get_mail_array(mail_array* array);
unsigned long int get_mail_array_size(mail_array* array);
int create_mail_array(mail_array* array);
int insert_mail_in_array(mail_array* array, mail* element);

#endif

#ifndef ___LIST_STRUCT___
#define ___LIST_STRUCT___

#include "date.h"
#include "cstring.h"
#include "cfile.h"
#include "cnumber.h"

typedef enum gender {
  MALE,
  FEMALE
} gender;

typedef struct villager {
  string* surname;
  string* name;
  string* middle_name;
  string* date;
  gender g;
  double salary;
} villager;

typedef villager* LIST_TYPE;

typedef struct list_node {
  struct list_node* next;
  LIST_TYPE data;
} list_node;

typedef struct list {
  struct list_node* head;
  unsigned long int size;
} list;

#endif

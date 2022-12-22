#ifndef ___LIST_STRUCT_INT___
#define ___LIST_STRUCT_INT___

#include "cstring.h"

#define LIST_INT_TYPE int

typedef struct list_int_node {
  struct list_int_node* next;
  LIST_INT_TYPE data;
} list_int_node;

typedef struct list_int {
  struct list_int_node* head;
  unsigned long int size;
} list_int;

#endif

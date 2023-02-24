#ifndef ___LIST_STRUCT___
#define ___LIST_STRUCT___

#include "cstring.h"

#define LIST_TYPE char

typedef struct list_node {
  struct list_node* next;
  LIST_TYPE data;
} list_node;

typedef struct list {
  struct list_node* head;
  unsigned long int size;
} list;

#endif

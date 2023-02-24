#ifndef ___CFIBO_STRUCT___
#define ___CFIBO_STRUCT___

#include "employee.h"

typedef employee* FIBO_T;

typedef struct fibo_node {
  FIBO_T value;
  struct fibo_node** brothers;
  unsigned long int brothers_count;
  unsigned long int real_brothers_size;
} fibo_node;

typedef fibo_node* LIST_T;

typedef struct list_node {
  LIST_T value;
  struct list_node* next;
  struct list_node* past;
} list_node;

typedef struct list {
  list_node* head;
  unsigned long int size;
} list;

typedef struct fibo {
  list_node* minimum;
  list* root_list;
  unsigned long int size;
} fibo;

#endif

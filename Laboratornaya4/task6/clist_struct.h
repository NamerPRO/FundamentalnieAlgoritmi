#ifndef ___LIST_STRUCT___
#define ___LIST_STRUCT___

#include "cstring.h"

typedef struct tree_node {
  struct tree_node* left;
  struct tree_node* right;
  sized_string* value;
} tree_node;

#define tree tree_node
#define LIST_TYPE tree_node*

typedef struct list_node {
  struct list_node* next;
  LIST_TYPE data;
} list_node;

typedef struct list {
  struct list_node* head;
  unsigned long int size;
} list;

#endif

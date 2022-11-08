#ifndef ___LIST_PRIORITY_QUEUE___
#define ___LIST_PRIORITY_QUEUE___

#include "t.h"
#include "cconstants.h"

typedef T LIST_QUEUE_T;

typedef struct list_queue_node {
  LIST_QUEUE_T* value;
  struct list_queue_node* next;
  struct list_queue_node* past;
} list_queue_node;

typedef struct list_queue {
  list_queue_node* head;
  unsigned long int size;
} list_queue;

void create_list_queue(list_queue* lst);
unsigned long int list_queue_size(list_queue* lst);
int list_queue_empty(list_queue* lst);
LIST_QUEUE_T* list_queue_get_minimum(list_queue* lq);
void list_queue_extract_min(list_queue* lq, void (*cls)(LIST_QUEUE_T* structure));
int standart_priority_queue_comporator(int lhs, int rhs);
int list_queue_insert(list_queue* lq, LIST_QUEUE_T* elem, int (*cmp)(int lhs, int rhs));
void standart_list_queue_free(LIST_QUEUE_T* structure);

#endif

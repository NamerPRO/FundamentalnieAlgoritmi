#ifndef ___CLIST___
#define ___CLIST___

#include <stdlib.h>
#include <stdarg.h>

#include "cconstants.h"
#include "cmonomial.h"
// #include "clist_struct.h"

#define LIST_T monomial*

typedef struct list_node {
  LIST_T value;
  struct list_node* next;
  struct list_node* past;
} list_node;

typedef struct list {
  list_node* head;
  unsigned long int size;
} list;

void create_list(list* lst);
unsigned long int list_size(list* lst);
int list_empty(list* lst);
int list_insert(list* lst, char* pattern, LIST_T elem, ...);
int list_push_front(list* lst, LIST_T elem);
int list_push_back(list* lst, LIST_T elem);
void standart_list_clear(list_node* elem);
int list_remove(list* lst, char* pattern, void (*clear)(list_node* elem), ...);
int list_remove_front(list* lst, void (*clear)(list_node* elem));
int list_remove_back(list* lst, void (*clear)(list_node* elem));
LIST_T list_extract_element(list* lst, char* pattern, int* execute_status, ...);
int list_free(list* lst, void (*clear)(list_node* elem));
list_node* list_extract_pointer(list* lst, char* pattern, int* execute_status, ...);

#endif

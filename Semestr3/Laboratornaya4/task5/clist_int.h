#ifndef ___LIST_INT___
#define ___LIST_INT___

#include <stdlib.h>

#include "cconstants.h"
#include "clist_struct_int.h"

void create_list_int(list_int* lst);
unsigned long int list_int_size(list_int* lst);
int list_int_empty(list_int* lst);
int list_int_push_front(list_int* lst, LIST_INT_TYPE elem);
int list_int_insert(list_int* lst, LIST_INT_TYPE elem, unsigned long int position);
int list_int_push_back(list_int* lst, LIST_INT_TYPE elem);
int list_int_pop_front(list_int* lst, void (*cls)(void* structure));
int list_int_pop_at(list_int* lst, unsigned long int position, void (*cls)(void* structure));
int list_int_pop_back(list_int* lst, void (*cls)(void* structure));
int list_int_clear(list_int* lst, void (*cls)(void* structure));
LIST_INT_TYPE list_int_element_at(list_int* lst, unsigned long int position, int* execute_status);

#endif

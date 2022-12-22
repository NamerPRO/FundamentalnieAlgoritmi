#ifndef ___LIST___
#define ___LIST___

#include <stdlib.h>

#include "cconstants.h"
#include "clist_struct.h"

void create_list(list* lst);
unsigned long int list_size(list* lst);
int list_empty(list* lst);
int list_push_front(list* lst, LIST_TYPE elem);
int list_insert(list* lst, LIST_TYPE elem, unsigned long int position);
int list_push_back(list* lst, LIST_TYPE elem);
int list_pop_front(list* lst, void (*cls)(void* structure));
int list_pop_at(list* lst, unsigned long int position, void (*cls)(void* structure));
int list_pop_back(list* lst, void (*cls)(void* structure));
int list_clear(list* lst, void (*cls)(void* structure));
LIST_TYPE list_element_at(list* lst, unsigned long int position, int* execute_status);
void standart_clear_function(void* structure);
void standart_string_clear_function(void* structure);

#endif

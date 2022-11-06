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
int list_pop_front(list* lst, void (*clear)(LIST_TYPE elem));
int list_pop_by(list* lst, string* s, void (*clear)(LIST_TYPE elem));
int list_clear(list* lst, void (*clear)(LIST_TYPE structure));
LIST_TYPE list_element_at(list* lst, unsigned long int position, int* execute_status);
void standart_clear_function(LIST_TYPE structure);

#endif

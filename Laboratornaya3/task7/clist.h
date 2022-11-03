#ifndef ___LIST___
#define ___LIST___

#include <stdlib.h>

#include "cconstants.h"
#include "cstring.h"
#include "clist_struct.h"

void create_list(list* lst);
unsigned long int list_size(list* lst);
int list_empty(list* lst);
int list_insert(list* lst, LIST_TYPE elem, int (*cmp)(LIST_TYPE lhs, LIST_TYPE rhs));
int list_pop_front(list* lst, void (*clear)(LIST_TYPE elem));
int list_pop_by(list* lst, string* surname, void (*clear)(LIST_TYPE elem));
int list_clear(list* lst, void (*clear)(LIST_TYPE structure));
LIST_TYPE list_element_at(list* lst, unsigned long int position, int* execute_status);
void standart_clear_function(LIST_TYPE structure);

#endif

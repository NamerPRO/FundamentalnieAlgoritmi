#ifndef ___STACK___
#define ___STACK___

#include "clist_struct.h"
#include "clist.h"

#define stack list
#define STACK_TYPE char

void create_stack(stack* st);
int stack_push(stack* st, STACK_TYPE elem);
unsigned long int stack_size(stack* st);
int stack_empty(stack* st);
int stack_pop(stack* st, void (*cls)(void* structure));
STACK_TYPE stack_top(stack* st, int* execute_status);
int stack_clear(stack* st, void (*cls)(void* structure));

#endif

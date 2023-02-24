#ifndef ___STACK_INT___
#define ___STACK_INT___

#include "clist_struct_int.h"
#include "clist_int.h"

#define stack_int list_int
#define STACK_INT_TYPE int

void create_stack_int(stack_int* st);
int stack_int_push(stack_int* st, STACK_INT_TYPE elem);
unsigned long int stack_int_size(stack_int* st);
int stack_int_empty(stack_int* st);
int stack_int_pop(stack_int* st, void (*cls)(void* structure));
STACK_INT_TYPE stack_int_top(stack_int* st, int* execute_status);
int stack_int_clear(stack_int* st, void (*cls)(void* structure));

#endif

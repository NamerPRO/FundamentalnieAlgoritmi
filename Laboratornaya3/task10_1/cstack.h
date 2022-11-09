#ifndef ___CSTACK___
#define ___CSTACK___

#include <stdlib.h>

#include "ctree.h"

typedef ctree T;

typedef struct element {
  T* value;
  struct element* past;
} element;

typedef struct stack {
  element* top;
  int size;
} stack;

void stack_create(stack* s);
int stack_size(stack* s);
int stack_empty(stack* s);
int stack_push(T* elem, stack* s);
T* stack_top(stack* s);
int stack_pop(stack* s, void (*cls)(T* structure));
void stack_destroy(stack* s, void (*cls)(T* structure));
void stack_clear_function(T* structure);

#endif

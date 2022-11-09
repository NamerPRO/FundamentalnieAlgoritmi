#include "cstack.h"

void stack_create(stack* s) {
  s->top = NULL;
  s->size = 0;
}

int stack_size(stack* s) {
  return s->size;
}

int stack_empty(stack* s) {
  return !s->size;
}

int stack_push(T* elem, stack* s) {
  element* e = s->top;
  if ((s->top = malloc(sizeof(element))) == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  s->top->past = e;
  s->top->value = elem;
  ++s->size;
  return SUCCESS_FUNCTION_RETURN;
}

T* stack_top(stack* s) {
  if(!stack_empty(s)) {
    return s->top->value;
  }
}

void stack_clear_function(T* structure) { }

int stack_pop(stack* s, void (*cls)(T* structure)) {
  if (stack_empty(s)) {
    return NO_SUCH_ELEMENT_EXCEPTION;
  }
  element* e = s->top;
  s->top = s->top->past;
  cls(e->value);
  free(e);
  --s->size;
  return SUCCESS_FUNCTION_RETURN;
}

void stack_destroy(stack* s, void (*cls)(T* structure)) {
  while (stack_pop(s, cls)) {}
}

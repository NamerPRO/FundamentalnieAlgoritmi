#include "stack.h"

void create_stack(stack* st) {
  create_list(st);
}

int stack_push(stack* st, STACK_TYPE elem) {
  return list_push_front(st, elem);
}

unsigned long int stack_size(stack* st) {
  return list_size(st);
}

int stack_empty(stack* st) {
  return list_empty(st);
}

int stack_pop(stack* st, void (*cls)(void* structure)) {
  return list_pop_front(st, cls);
}

STACK_TYPE stack_top(stack* st, int* execute_status) {
  return list_element_at(st, 0, execute_status);
}

int stack_clear(stack* st, void (*cls)(void* structure)) {
  return list_clear(st, cls);
}

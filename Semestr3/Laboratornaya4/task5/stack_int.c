#include "stack_int.h"

void create_stack_int(stack_int* st) {
  create_list_int(st);
}

int stack_int_push(stack_int* st, STACK_INT_TYPE elem) {
  return list_int_push_front(st, elem);
}

unsigned long int stack_int_size(stack_int* st) {
  return list_int_size(st);
}

int stack_int_empty(stack_int* st) {
  return list_int_empty(st);
}

int stack_int_pop(stack_int* st, void (*cls)(void* structure)) {
  return list_int_pop_front(st, cls);
}

STACK_INT_TYPE stack_int_top(stack_int* st, int* execute_status) {
  return list_int_element_at(st, 0, execute_status);
}

int stack_int_clear(stack_int* st, void (*cls)(void* structure)) {
  return list_int_clear(st, cls);
}

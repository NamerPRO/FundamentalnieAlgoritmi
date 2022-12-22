#include "clist_int.h"

void create_list_int(list_int* lst) {
  lst->head = NULL;
  lst->size = 0;
}

unsigned long int list_int_size(list_int* lst) {
  return lst->size;
}

int list_int_empty(list_int* lst) {
  return !list_int_size(lst);
}

int list_int_push_front(list_int* lst, LIST_INT_TYPE elem) {
  list_int_node* lst_node = (list_int_node*)malloc(sizeof(list_int_node));
  if (lst_node == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  lst_node->data = elem;
  lst_node->next = lst->head;
  lst->head = lst_node;
  ++lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_int_insert(list_int* lst, LIST_INT_TYPE elem, unsigned long int position) {
  if (position == 0) {
    return list_int_push_front(lst, elem);
  }
  if (position > list_int_size(lst)) {
    return ARGUMENTS_EXCEPTION;
  }
  list_int_node* lst_node = lst->head;
  for (unsigned long int i = 0; i < position - 1; ++i) {
    lst_node = lst_node->next;
  }
  list_int_node* lst_node_new = (list_int_node*)malloc(sizeof(list_int_node));
  if (lst_node_new == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  lst_node_new->data = elem;
  lst_node_new->next = lst_node->next;
  lst_node->next = lst_node_new;
  ++lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_int_push_back(list_int* lst, LIST_INT_TYPE elem) {
  return list_int_insert(lst, elem, list_int_size(lst));
}

int list_int_pop_front(list_int* lst, void (*cls)(void* structure)) {
  if (list_int_empty(lst)) {
    return ARGUMENTS_EXCEPTION;
  }
  list_int_node* lst_node = lst->head;
  lst->head = lst->head->next;
  --lst->size;
  // cls(lst_node->data);
  free(lst_node);
  return SUCCESS_FUNCTION_RETURN;
}

int list_int_pop_at(list_int* lst, unsigned long int position, void (*cls)(void* structure)) {
  if (position > list_int_size(lst)) {
    return ARGUMENTS_EXCEPTION;
  }
  if (position == 0) {
    return list_int_pop_front(lst, cls);
  }
  list_int_node* lst_node = lst->head;
  for (unsigned long int i = 0; i < position - 1; ++i) {
    lst_node = lst_node->next;
  }
  list_int_node* lst_node_new = lst_node->next->next;
  cls(lst_node->data);
  free(lst_node->next);
  lst_node->next = lst_node_new;
  --lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_int_pop_back(list_int* lst, void (*cls)(void* structure)) {
  return list_int_pop_at(lst, list_int_size(lst) - 1, cls);
}

int list_int_clear(list_int* lst, void (*cls)(void* structure)) {
  int execute_status;
  while (!list_int_empty(lst)) {
    void* elem = list_int_element_at(lst, 0, &execute_status);
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    cls(elem);
    if ((execute_status = list_int_pop_front(lst, cls)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

LIST_INT_TYPE list_int_element_at(list_int* lst, unsigned long int position, int* execute_status) {
  if (position >= list_int_size(lst)) {
    *execute_status = ARGUMENTS_EXCEPTION;
    return lst->head->data;
  }
  list_int_node* lst_node = lst->head;
  for (unsigned long int i = 0; i < position; ++i) {
    lst_node = lst_node->next;
  }
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return lst_node->data;
}

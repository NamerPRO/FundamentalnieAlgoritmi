#include "clist.h"

void create_list(list* lst) {
  lst->head = NULL;
  lst->size = 0;
}

unsigned long int list_size(list* lst) {
  return lst->size;
}

int list_empty(list* lst) {
  return !list_size(lst);
}

int list_push_front(list* lst, LIST_TYPE elem) {
  list_node* lst_node = (list_node*)malloc(sizeof(list_node));
  if (lst_node == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  lst_node->data = elem;
  lst_node->next = lst->head;
  lst->head = lst_node;
  ++lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_insert(list* lst, LIST_TYPE elem, unsigned long int position) {
  if (position == 0) {
    return list_push_front(lst, elem);
  }
  if (position > list_size(lst)) {
    return ARGUMENTS_EXCEPTION;
  }
  list_node* lst_node = lst->head;
  for (unsigned long int i = 0; i < position - 1; ++i) {
    lst_node = lst_node->next;
  }
  list_node* lst_node_new = (list_node*)malloc(sizeof(list_node));
  if (lst_node_new == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  lst_node_new->data = elem;
  lst_node_new->next = lst_node->next;
  lst_node->next = lst_node_new;
  ++lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_push_back(list* lst, LIST_TYPE elem) {
  return list_insert(lst, elem, list_size(lst));
}

int list_pop_front(list* lst, void (*cls)(void* structure)) {
  if (list_empty(lst)) {
    return ARGUMENTS_EXCEPTION;
  }
  list_node* lst_node = lst->head;
  lst->head = lst->head->next;
  --lst->size;
  // cls(lst_node->data);
  free(lst_node);
  return SUCCESS_FUNCTION_RETURN;
}

int list_pop_at(list* lst, unsigned long int position, void (*cls)(void* structure)) {
  if (position > list_size(lst)) {
    return ARGUMENTS_EXCEPTION;
  }
  if (position == 0) {
    return list_pop_front(lst, cls);
  }
  list_node* lst_node = lst->head;
  for (unsigned long int i = 0; i < position - 1; ++i) {
    lst_node = lst_node->next;
  }
  list_node* lst_node_new = lst_node->next->next;
  cls(lst_node->data);
  free(lst_node->next);
  lst_node->next = lst_node_new;
  --lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_pop_back(list* lst, void (*cls)(void* structure)) {
  return list_pop_at(lst, list_size(lst) - 1, cls);
}

void standart_clear_function(void* structure) { }

void standart_string_clear_function(void* structure) {
  free_heap_string((string*)structure);
}

int list_clear(list* lst, void (*cls)(void* structure)) {
  int execute_status;
  while (!list_empty(lst)) {
    void* elem = list_element_at(lst, 0, &execute_status);
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    cls(elem);
    if ((execute_status = list_pop_front(lst, cls)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

LIST_TYPE list_element_at(list* lst, unsigned long int position, int* execute_status) {
  if (position >= list_size(lst)) {
    *execute_status = ARGUMENTS_EXCEPTION;
    return lst->head->data;
  }
  list_node* lst_node = lst->head;
  for (unsigned long int i = 0; i < position; ++i) {
    lst_node = lst_node->next;
  }
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return lst_node->data;
}

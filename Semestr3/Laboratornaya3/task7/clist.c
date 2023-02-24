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

int list_insert(list* lst, LIST_TYPE elem, int (*cmp)(LIST_TYPE lhs, LIST_TYPE rhs)) {
  if (list_empty(lst)) {
    return list_push_front(lst, elem);
  }
  list_node* prev_lst_node = lst->head;
  list_node* cur_lst_node = lst->head->next;
  if (cmp(prev_lst_node->data, elem) == 1) {
    return list_push_front(lst, elem);
  }
  while (cur_lst_node != NULL && (cmp(cur_lst_node->data, elem) == -1)) {
    prev_lst_node = cur_lst_node;
    cur_lst_node = cur_lst_node->next;
  }
  list_node* lst_node_new = (list_node*)malloc(sizeof(list_node));
  if (lst_node_new == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  lst_node_new->data = elem;
  lst_node_new->next = cur_lst_node;
  prev_lst_node->next = lst_node_new;
  ++lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_pop_front(list* lst, void (*clear)(LIST_TYPE elem)) {
  if (list_empty(lst)) {
    return ARGUMENTS_EXCEPTION;
  }
  list_node* lst_node = lst->head->next;
  clear(lst->head->data);
  free(lst->head);
  lst->head = lst_node;
  --lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_pop_by(list* lst, string* surname, void (*clear)(LIST_TYPE elem)) {
  list_node* prev_lst_node = lst->head;
  list_node* cur_lst_node = lst->head->next;
  if (!string_compare("%s", standart_string_comporator, surname, prev_lst_node->data->surname)) {
    return list_pop_front(lst, clear);
  }
  while (cur_lst_node != NULL && string_compare("%s", standart_string_comporator, surname, cur_lst_node->data->surname)) {
    prev_lst_node = cur_lst_node;
    cur_lst_node = cur_lst_node->next;
  }
  if (cur_lst_node == NULL) {
    return WRONG_INPUT_EXCEPTION;
  }
  list_node* lst_node_new = cur_lst_node->next;
  clear(cur_lst_node->data);
  free(cur_lst_node);
  prev_lst_node->next = lst_node_new;
  --lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

void standart_clear_function(LIST_TYPE structure) { }

int list_clear(list* lst, void (*clear)(LIST_TYPE structure)) {
  int execute_status;
  while (!list_empty(lst)) {
    // LIST_TYPE elem = list_element_at(lst, 0, &execute_status);
    // if (execute_status != SUCCESS_FUNCTION_RETURN) {
    //   return execute_status;
    // }
    // clear(elem);
    if ((execute_status = list_pop_front(lst, clear)) != SUCCESS_FUNCTION_RETURN) {
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

#include "list_priority_queue.h"

void create_list_queue(list_queue* lst) {
  lst->head = NULL;
  lst->size = 0;
}

unsigned long int list_queue_size(list_queue* lst) {
  return lst->size;
}

int list_queue_empty(list_queue* lst) {
  return !lst->size;
}

LIST_QUEUE_T* list_queue_get_minimum(list_queue* lq) {
  return lq->head->value;
}

void standart_list_queue_free(LIST_QUEUE_T* structure) {
  free_string(structure->text);
  free(structure->text);
  free(structure);
}

void list_queue_extract_min(list_queue* lq, void (*cls)(LIST_QUEUE_T* structure)) {
  if (!list_queue_empty(lq)) {
    list_queue_node* lqn = lq->head->next;
    list_queue_node* lqn_past = lq->head->past;
    lqn->past = lqn_past;
    lqn_past->next = lqn;
    cls(lq->head->value);
    free(lq->head);
    lq->head = lqn;
    --lq->size;
  }
}

int standart_priority_queue_comporator(int lhs, int rhs) {
  return (lhs <= rhs) ? 1 : 0;
}

int list_queue_insert(list_queue* lq, LIST_QUEUE_T* elem, int (*cmp)(int lhs, int rhs)) {
  if (list_queue_empty(lq)) {
    lq->head = (list_queue_node*)malloc(sizeof(list_queue_node));
    if (lq->head == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    lq->head->value = elem;
    lq->head->next = lq->head;
    lq->head->past = lq->head;
    lq->size = 1;
    return SUCCESS_FUNCTION_RETURN;
  }
  list_queue_node* lqn_node = (list_queue_node*)malloc(sizeof(list_queue_node));
  if (lqn_node == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  lqn_node->value = elem;
  list_queue_node* lqn = lq->head;
  unsigned long int i = 0;
  for (; i < list_queue_size(lq); ++i) {
    if (cmp(elem->priority, lqn->value->priority)) {
      lqn = lqn->next;
      continue;
    }
    break;
  }
  lqn = lqn->past;
  list_queue_node* lqn_save = lqn->next;
  lqn->next = lqn_node;
  lqn_node->past = lqn;
  lqn_node->next = lqn_save;
  lqn_save->past = lqn_node;
  if (!i) { //lqn->next == lq->head->past
    lq->head = lq->head->past;
  }
  ++lq->size;
  return SUCCESS_FUNCTION_RETURN;
}

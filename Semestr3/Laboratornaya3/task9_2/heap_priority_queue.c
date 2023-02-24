#include "heap_priority_queue.h"

void create_heap_queue(heap_queue* hq) {
  hq->size = 0;
  hq->array = NULL;
}

int heap_queue_size(heap_queue* hq) {
  return hq->size;
}

int heap_queue_empty(heap_queue* hq) {
  return !hq->size;
}

HEAP_QUEUE_T* heap_queue_get_maximum(heap_queue* hq) {
  return heap_queue_empty(hq) ? NULL : hq->array[0];
}

int standart_sift_comporator(int lhs, int rhs) {
  return (lhs > rhs) ? 1 : 0;
}

void heap_queue_sift_down(heap_queue* hq, int (*cmp)(int lhs, int rhs)) {
  int i = 0, to_swap;
  int left, right;
  while (2 * i + 1 < hq->size) {
    left = 2 * i + 1;
    right = 2 * i + 2;
    to_swap = left;
    if (right < hq->size && cmp(hq->array[right]->priority, hq->array[left]->priority)) {
      to_swap = right;
    }
    if (cmp(hq->array[i]->priority, hq->array[to_swap]->priority)) {
      break;
    }
    T* save_t = hq->array[i];
    hq->array[i] = hq->array[to_swap];
    hq->array[to_swap] = save_t;
    i = to_swap;
  }
}

void standart_heap_queue_clear(HEAP_QUEUE_T* structure) {
  free_string(structure->text);
  free(structure->text);
}

void heap_queue_extract_max(heap_queue* hq, int (*cmp)(int lhs, int rhs), void (*cls)(HEAP_QUEUE_T* structure)) {
  --hq->size;
  cls(hq->array[0]);
  free(hq->array[0]);
  hq->array[0] = hq->array[hq->size];
  heap_queue_sift_down(hq, cmp);
}

void heap_queue_destroy(heap_queue* hq, int (*cmp)(int lhs, int rhs), void (*cls)(HEAP_QUEUE_T* structure)) {
  while (!heap_queue_empty(hq)) {
    heap_queue_extract_max(hq, cmp, cls);
  }
  free(hq->array);
}

void heap_queue_sift_up(heap_queue* hq, int i, int (*cmp)(int lhs, int rhs)) {
  int key = (i - 1) / 2;
  while (cmp(hq->array[i]->priority, hq->array[key]->priority)) {
    T* save_t = hq->array[i];
    hq->array[i] = hq->array[key];
    hq->array[key] = save_t;
    i = key;
    key = (i - 1) / 2;
  }
}

int heap_queue_insert(heap_queue* hq, HEAP_QUEUE_T* elem, int (*cmp)(int lhs, int rhs)) {
  HEAP_QUEUE_T** possible_array = (HEAP_QUEUE_T**)realloc(hq->array, sizeof(HEAP_QUEUE_T*) * (hq->size + 1));
  if (possible_array == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  hq->array = possible_array;
  hq->array[hq->size] = elem;
  heap_queue_sift_up(hq, hq->size, cmp);
  ++hq->size;
  return SUCCESS_FUNCTION_RETURN;
}

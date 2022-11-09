#ifndef ___HEAP_PRIORITY_QUEUE___
#define ___HEAP_PRIORITY_QUEUE___

#include "t.h"
#include "cconstants.h"

typedef T HEAP_QUEUE_T;

typedef struct heap_queue {
  int size;
  HEAP_QUEUE_T** array;
} heap_queue;

void create_heap_queue(heap_queue* hq);
int heap_queue_size(heap_queue* hq);
int heap_queue_empty(heap_queue* hq);
HEAP_QUEUE_T* heap_queue_get_maximum(heap_queue* hq);
int standart_sift_comporator(int lhs, int rhs);
void heap_queue_sift_down(heap_queue* hq, int (*cmp)(int lhs, int rhs));
void heap_queue_extract_max(heap_queue* hq, int (*cmp)(int lhs, int rhs), void (*cls)(HEAP_QUEUE_T* structure));
void heap_queue_sift_up(heap_queue* hq, int i, int (*cmp)(int lhs, int rhs));
int heap_queue_insert(heap_queue* hq, HEAP_QUEUE_T* elem, int (*cmp)(int lhs, int rhs));
void heap_queue_destroy(heap_queue* hq, int (*cmp)(int lhs, int rhs), void (*cls)(HEAP_QUEUE_T* structure));
void standart_heap_queue_clear(HEAP_QUEUE_T* structure);

#endif

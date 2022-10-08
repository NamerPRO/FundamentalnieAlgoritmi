#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEMORY_EXCEPTION 1
#define WRONG_ACTION_EXCEPTION 2
#define WRONG_INPUT_EXCEPTION 3
#define PROHIBITED_SIZE_EXCEPTION 4

void swap(int* a, int* b) {
  int c = *a;
  *a = *b;
  *b = c;
}

void print_array(int* array, int size) {
  for (int i = 0; i < size; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int main() {
  int size;
  printf("Enter array size: ");
  scanf("%d", &size);
  if (size <= 0) {
    return PROHIBITED_SIZE_EXCEPTION;
  }
  int* array = NULL;
  if ((array = malloc(sizeof(int) * size)) == NULL) {
    return MEMORY_EXCEPTION;
  }
  int left_boundary;
  int right_boundary;
  printf("Specify the boundaries of the segment (left, right): ");
  scanf("%d%d", &left_boundary, &right_boundary);
  if (left_boundary > right_boundary) {
    return WRONG_INPUT_EXCEPTION;
  }
  srand(time(NULL));
  for (int i = 0; i < size; ++i) {
    array[i] = (rand() % (right_boundary - left_boundary + 1)) + left_boundary;
  }
  printf("Generated array: ");
  print_array(array, size);
  printf("====================\n");
  printf("Choose an action now:\n");
  printf("1 = swap min and max elements.\n");
  printf("2 = remove repeat elements from array.\n");
  printf("====================\n");
  int action;
  scanf("%d", &action);
  switch (action) {
    case 1: ;
      int index_max = 0;
      int index_min = 0;
      for (int i = 1; i < size; ++i) {
        if (array[index_max] < array[i]) {
          index_max = i;
        }
        if (array[index_min] > array[i]) {
          index_min = i;
        }
      }
      swap(array + index_max, array + index_min);
      printf("Result looks like: ");
      print_array(array, size);
      break;
    case 2: ;
      int* new_array = malloc(sizeof(int) * size);
      int element_exits = 0;
      int new_array_size = 0;
      for (int i = 0; i < size; ++i) {
        element_exits = 0;
        for (int j = 0; j < i; ++j) {
          if (array[i] == array[j]) {
            element_exits = 1;
            break;
          }
        }
        if (!element_exits) {
          new_array[new_array_size++] = array[i];
        }
      }
      printf("Result looks like: ");
      print_array(new_array, new_array_size);
      free(new_array);
      break;
    default:
      free(array);
      return WRONG_ACTION_EXCEPTION;
  }
  free(array);
  return 0;
}

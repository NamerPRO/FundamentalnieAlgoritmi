#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bin_search.h"
#include "qsort.h"

int rnd = 1;

void fill_arr_integer(int* array, int size) {
  srand(time(NULL) + rnd);
  for (int i = 0; i < size; ++i) {
    array[i] = rand() % 20 - 10;
  }
  rnd = array[size - 1];
}

void print_array(int* array, int size) {
  for (int i = 0; i < size; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int main() {
  int size_a, size_b;
  printf("Enter size of array A and array B: ");
  scanf("%d%d", &size_a, &size_b);

  int* a = (int*)malloc(sizeof(int) * size_a);
  int* b = (int*)malloc(sizeof(int) * size_b);

  fill_arr_integer(a, size_a);
  printf("Generated array A looks like this: ");
  print_array(a, size_a);

  fill_arr_integer(b, size_b);
  printf("Generated array B looks like this: ");
  print_array(b, size_b);

  sort(b, 0, size_b - 1);

  int size_c = 2 * size_a;
  int* c = (int*)malloc(sizeof(int) * size_c);

  int temp, j = 0;
  for (int i = 0; i < size_c; i += 2) {
    temp = bin_search(b, a[j], 0, size_b - 1);
    c[i] = a[j++];
    c[i + 1] = temp;
  }

  printf("Result array C looks like this: ");
  print_array(c, size_c);

  free(a);
  free(b);
  free(c);

  return 0;
}

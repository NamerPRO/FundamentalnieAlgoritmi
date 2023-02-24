#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE_EXCEPTION 1

void print_array(int* array, int n) {
  for (int i = 0; i < n; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int main() {
  int n;
  printf("Fix size of array: ");
  scanf("%d", &n);

  if (n < 0) {
    return ARRAY_SIZE_EXCEPTION;
  }

  int* array = (int*)malloc(sizeof(int) * n);
  for (int i = 0; i < n; ++i) {
    array[i] = rand() % 100;
  }

  printf("Before: ");
  print_array(array, n);

  if (n > 1) {
    int m_max = -1;
    int max_i;
    int m_min = 101;
    int min_i;
    for (int i = 0; i < n; ++i) {
      if (array[i] > m_max) {
        m_max = array[i];
        max_i = i;
      }
      if (array[i] < m_min) {
        m_min = array[i];
        min_i = i;
      }
    }

    array[min_i] = m_max;
    array[max_i] = m_min;

    printf("After: ");
    print_array(array, n);
  } else {
    printf("After: ");
    print_array(array, n);
  }

  free(array);

  return 0;
}

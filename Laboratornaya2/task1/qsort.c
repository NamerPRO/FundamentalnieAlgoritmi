#include "qsort.h"

void swap(int* x, int* y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

int partition(int* array, int left, int right) {
  int middle_element = array[(left + right) / 2];
  while (left <= right) {
    while (array[left] < middle_element) {
      ++left;
    }
    while (array[right] > middle_element) {
      --right;
    }
    if (left >= right) {
      break;
    }
    swap(array + left++, array + right--);
  }
  return right;
}

int* sort(int* array, int left, int right) {
  if (left < right) {
    int split = partition(array, left, right);
    sort(array, left, split);
    sort(array, split + 1, right);
  }
}

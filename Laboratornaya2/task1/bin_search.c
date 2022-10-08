#include "bin_search.h"

int bin_search(int* array, int x, int left, int right) {
  int result = MAX_NUMBER;
  while (left <= right) {
    int middle = (left + right) / 2; //(right - left) / 2 + left;
    if (abs(x - array[middle]) < abs(x - result)) {
      result = array[middle];
    }
    if (array[middle] > x) {
      right = middle - 1;
    } else if (array[middle] < x) {
      left = middle + 1;
    } else {
      return result;
    }
  }
  return result;
}

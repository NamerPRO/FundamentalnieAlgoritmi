#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "overflow.h"

#define ARGUMENTS_EXCEPTION 1
#define FILE_EXCEPTION 2
#define INPUT_EXCEPTION 3
#define VARIABLE_OVERFLOW_EXCEPTION 4
#define WRONG_INPUT_INDEX_EXCEPTION 5
#define UNKNOWN_ACTION_EXCEPTION 6

void print_array(int* array, int size) {
  for (int i = 0; i < size; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int char_to_digit(char c) {
  return c - '0';
}

int my_strtoi(char* str, int* is_number) {
  int is_negative = 1;
  unsigned int i = 0;
  if (*str == '-') {
    is_negative = -1;
    i = 1;
  }
  int int_number = 0;
  int is_overflowed;
  for (; i < strlen(str); ++i) {
    if (!isdigit(str[i])) {
      *is_number = 0;
      return 0;
    }
    int_number = safe_mult(int_number, 10, &is_overflowed);
    if (is_overflowed) {
      *is_number = 0;
      return 0;
    }
    int_number = safe_sum(int_number, char_to_digit(str[i]), &is_overflowed);
    if (is_overflowed) {
      *is_number = 0;
      return 0;
    }
  }
  *is_number = 1;
  return int_number * is_negative;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  FILE* input_file = NULL;
  if ((input_file = fopen(argv[1], "r")) == NULL) {
    return FILE_EXCEPTION;
  }
  int array[128];
  int array_size = 0;
  int is_number;
  char unsafe_number_string[255];
  while (fscanf(input_file, "%s", unsafe_number_string) == 1) {
    array[array_size++] = my_strtoi(unsafe_number_string, &is_number);
    if (!is_number) {
      return INPUT_EXCEPTION;
    }
  }
  fclose(input_file);
  printf("Before array: ");
  print_array(array, array_size);
  char symbol;
  printf("Enter an action from a given list:");
  scanf("%c", &symbol);
  int current_element_index;
  if (symbol >= 'c' && symbol <= 'e') {
    scanf("%d", &current_element_index);
    if (current_element_index < 0 || current_element_index >= array_size) {
      return WRONG_INPUT_INDEX_EXCEPTION;
    }
  }
  int new_array[128];
  int j = 0;
  switch (symbol) {
    case 'a':
      for (int i = 1; i < array_size; i += 2) {
        new_array[j++] = array[i];
      }
      printf("After array: ");
      print_array(new_array, j);
      break;
    case 'b':
      for (int i = 0; i < array_size; ++i) {
        if (array[i] % 2 == 0) {
          new_array[j++] = array[i];
        }
      }
      printf("After array: ");
      print_array(new_array, j);
      break;
    case 'c': ;
      int delta = -1;
      int furthest_index;
      if (array_size > 1) {
        for (int i = 0; i < array_size; ++i) {
          if (i == current_element_index) {
            continue;
          }
          if (delta < abs(array[i] - array[current_element_index])) {
            delta = abs(array[i] - array[current_element_index]);
            furthest_index = i;
          }
        }
        printf("Furthest element from element %d by value is element %d located at position %d in array.\n", array[current_element_index], array[furthest_index], furthest_index);
      } else {
        printf("Array that consists of one element found! So there is no element that can be further than it.\n");
      }
      break;
    case 'd': ;
      int sum_before = 0;
      int is_overflowed_sum_before;
      for (int i = 0; i < current_element_index; ++i) {
        sum_before = safe_sum(sum_before, array[i], &is_overflowed_sum_before);
        if (is_overflowed_sum_before) {
          return VARIABLE_OVERFLOW_EXCEPTION;
        }
      }
      printf("Queried sum is %d.\n", sum_before);
      break;
    case 'e': ;
      int sum_less = 0;
      int is_overflowed_sum_less;
      for (int i = 0; i < array_size; ++i) {
        if (array[i] < array[current_element_index]) {
          sum_less = safe_sum(sum_less, array[i], &is_overflowed_sum_less);
          if (is_overflowed_sum_less) {
            return VARIABLE_OVERFLOW_EXCEPTION;
          }
        }
      }
      printf("Queried sum is %d.\n", sum_less);
      break;
    default:
      return UNKNOWN_ACTION_EXCEPTION;
  }
}

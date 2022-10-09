#include <stdio.h>
#include <stdlib.h>

#include "cinteger.h"
#include "coverflow.h"

#define STANDART_STRING_SIZE 35
#define CONVERTATION_EXCEPTION 1

typedef struct cnumber {
  char* real_start;
  char* data_start;
} cnumber;

char to_system(int symbol) {
  return (symbol < 10) ? symbol + '0' : symbol - 10 + 'A';
}

cnumber convert(int number, int r) {
  char* buffer = (char*)malloc(sizeof(char) * STANDART_STRING_SIZE);
  if (buffer == NULL) {
    return (cnumber){ .real_start = NULL, .data_start = NULL };
  }
  char* buffer_ptr = buffer + STANDART_STRING_SIZE - 1;
  *buffer_ptr = '\0';
  int mask = (1 << r) - 1;
  do {
    *--buffer_ptr = to_system(number & mask);
  } while (number = (number >> r));
  return (cnumber){ .real_start = buffer, .data_start = buffer_ptr };
}

char* get_number(cnumber number) {
  return number.data_start;
}

int has_number(cnumber number) {
  return (number.real_start == NULL) ? 0 : 1;
}

void free_number(cnumber number) {
  free(number.real_start);
}

int main() {
  int number, r;
  printf("Enter a number and r:");
  scanf("%d%d", &number, &r);
  cnumber converted_number = convert(number, r);
  if (has_number(converted_number)) {
    printf("Converted number = %s\n", get_number(converted_number));
    free_number(converted_number);
  } else {
    return CONVERTATION_EXCEPTION;
  }
  return 0;
}

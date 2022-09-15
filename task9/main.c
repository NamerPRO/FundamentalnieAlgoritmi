#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decimal.h"
#include "overflow.h"

#define INPUT_EXCEPTION 1

int max(const int a, const int b) {
  return (a > b) ? a : b;
}

int main() {
  int system;
  printf("Enter system:");
  scanf("%d", &system);

  if (system > 36 || system < 2) {
    return INPUT_EXCEPTION;
  }

  char string_input[255];
  int dec_number;
  int max_number = 0;
  int is_flagged;
  while(1) {
    scanf("%s", string_input);
    if (!strcmp(string_input, "Stop\0")) {
      break;
    }
    if (!is_number_in_required_system(string_input, system)) {
      return INPUT_EXCEPTION;
    }
    dec_number = safe_to_dec(string_input, system, &is_flagged);
    if (is_flagged) {
      return INPUT_EXCEPTION;
    }
    if (abs(max_number) < abs(dec_number)) {
      max_number = dec_number;
    }
  }

  printf("Maximum by moduly number in different systems:\n");
  printf("9:  %s\n", dec_to_system(max_number, 9, string_input));
  printf("18: %s\n", dec_to_system(max_number, 18, string_input));
  printf("27: %s\n", dec_to_system(max_number, 27, string_input));
  printf("36: %s\n", dec_to_system(max_number, 36, string_input));

  return 0;
}

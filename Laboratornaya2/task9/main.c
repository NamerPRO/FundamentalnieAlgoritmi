#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define START_NUM_SIZE 255

char to_dec(char digit) {
  digit = toupper(digit);
  return (digit >= 'A') ? digit - 'A' + 10 : digit - '0';
}

char to_system(int number) {
  return (number > 9) ? number - 10 + 'A' : number + '0';
}

// char* remove_top_zeros(char* str) {
//   int i = 0;
//   for (; str[i] == '0'; ++i) {}
//   return str + i;
// }

//Убрать ведущие нули
//Динамическую строку +
//remember=1 +
char* add(int system, char* number1, char* number2, char** buffer) {
  int length1 = strlen(number1);
  int length2 = strlen(number2);
  if (length1 < length2) {
    char* number3 = number1;
    number1 = number2;
    number2 = number3;
    int length3 = length1;
    length1 = length2;
    length2 = length3;
  }
  static unsigned long int current_allocated_buffer_space = START_NUM_SIZE;
  while ((length1 + 2) * sizeof(char) > current_allocated_buffer_space) {
    current_allocated_buffer_space *= 2;
    char* possible_buffer = (char*)realloc(*buffer, sizeof(char) * current_allocated_buffer_space);
    if (possible_buffer == NULL) {
      return NULL;
    }
    *buffer = possible_buffer;
  }
  int j = length2;
  char* buffer_ptr = *buffer + current_allocated_buffer_space - 1;
  *buffer_ptr = '\0';
  int remember = 0;
  for (int i = length1 - 1; i >= -1; --i) {
    int digit1 = (i >= 0) ? to_dec(number1[i]) : 0;
    int digit2 = (j > 0) ? to_dec(number2[--j]) : 0;
    int digit_sum = digit1 + digit2 + remember;
    if (digit_sum < system) {
      remember = 0;
      if (i == -1 && !digit_sum) {
        break;
      }
      *--buffer_ptr = to_system(digit_sum);
    } else {
      remember = 1; //digit_sum / system
      *--buffer_ptr = to_system(digit_sum % system);
    }
  }
  return buffer_ptr;
}

char* get_sum(char** buffer, int system, int count, ...) {
  va_list numbers;
  va_start(numbers, count);
  char* previous_number = va_arg(numbers, char*);
  for (int i = 1; i < count; ++i) {
    char* current_number = va_arg(numbers, char*);
    previous_number = add(system, previous_number, current_number, buffer);
  }
  return previous_number;
}

int main() {
  // char buffer[MAX_SIZE];
  char* buffer = (char*)malloc(sizeof(char) * START_NUM_SIZE);
  printf("Calculated sum: %s\n", get_sum(&buffer, 17, 6, "gaggabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgababbggabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabagabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabbgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabgabggababgab", "gfgfgfggfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgffgfgfgfgfgfgfgfgfgfgfgfgfgfgfggfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgffgfgfgfgfgfgfgfgfgfgfgfgfgfgfggfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgffgfgfgfgfgfgfgfgfgfgfgfgfgfgfggfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgffgfgfgfgfgfgfgfgfgfgfgf", "b1gfgfgfggfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgf123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123gfgfgfgfgfgfgfgfgfgfgfgfgfgfgffgfgfgfgfgfgfgfgfgfgfgfgfgfgfggfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgffgfgfgfgfgfgfgfgfgfgfgfgfgfgfggfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgffgfgfgfgfgfgfgfgfgfgfgfgfgfgfggfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfgfg123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123123fgfgfgfgfgfgfgfgffgfgfgfgfgfgfgfgfgfgfgf2c", "123123123123123123123123123123123123123123123123123123", "0", "babadgbadgebadgebadgeebadbadgebadgebadgebadgbadgebadgebadgebadgebadgeebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebabadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgebadgedgebadgebadgebadgebadgebadgebadgebbadgebadgebadgebadgeadgegebbadgebadgebadgeadgebadgebadgebadgedge"));
  free(buffer);
  return 0;
}

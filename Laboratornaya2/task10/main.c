#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_SIZE 255

char* get_denominator(char* fraction) {
  int i = 0;
  for (; fraction[i] != '/'; ++i) {}
  return fraction + i + 1;
}

int is_periodic(int denominator) {
  for (int i = 3; i <= denominator; i += 2) {
    if ((i % 5 > 0) && (denominator % i == 0)) {
      return 1;
    }
  }
  return 0;
}

int main() {
  char fraction[MAX_SIZE];
  printf("Enter fractions below to determine whether they are periodic or not:\n");
  while (scanf("%s", fraction) == 1) {
    int denominator = atoi(get_denominator(fraction));
    if (is_periodic(denominator)) {
      printf("Fraction is periodic!\n");
    } else {
      printf("Fraction is NOT periodic!\n");
    }
  }
  return 0;
}

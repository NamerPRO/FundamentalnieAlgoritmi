#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_SIZE 255
#define EPSILON 0.000001
#define HAS_END 1
#define ENDLESS 0
#define MEMORY_ALLOCATE_EXCEPTION 2

typedef struct frac {
  int numerator;
  int denominator;
} frac;

int* insert_in_array(int number, int* res_arr, int* current_size, int* real_current_size) {
  if (*real_current_size == *current_size) {
    int* possible_res_arr = (int*)realloc(res_arr, sizeof(int) * 2 * *current_size);
    if (possible_res_arr == NULL) {
      return NULL;
    }
    res_arr = possible_res_arr;
    *current_size *= 2;
  }
  res_arr[(*real_current_size)++] = number;
  return res_arr;
}

int contains_in_arr(int* arr, int elem, int size) {
  for (int i = 0; i < size; ++i) {
    if (arr[i] == elem) {
      return 1;
    }
  }
  return 0;
}

int is_periodic(frac fraction, int system) {
  int* res_arr = (int*)malloc(sizeof(int) * 10);
  if (res_arr == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  int real_current_size = 0;
  int current_size = 10;
  int ost = fraction.numerator % fraction.denominator;
  while (1) {
    if (insert_in_array(ost, res_arr, &current_size, &real_current_size) == NULL) {
      free(res_arr);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    ost = (ost * system) % fraction.denominator;
    if (ost == 0) {
      free(res_arr);
      return HAS_END;
    }
    if (contains_in_arr(res_arr, ost, current_size)) {
      free(res_arr);
      return ENDLESS;
    }
  }
}

int gcd(int x1, int x2) {
  while (x1 != x2) {
    if (x1 > x2) {
      x1 -= x2;
    } else {
      x2 -= x1;
    }
  }
  return x1;
}

frac to_fraction(double dec_frac, const double epsilon) {
  int num = 1;
  double denom;
  double previous_frac = modf(dec_frac, &denom);
  double current_frac;
  num *= 10;
  dec_frac *= 10;
  while (fabs((current_frac = modf(dec_frac, &denom)) - previous_frac) > epsilon) {
    num *= 10;
    dec_frac *= 10;
    previous_frac = current_frac;
  }
  int dec_frac_int = dec_frac;
  int div = gcd(num, dec_frac_int);
  num /= div;
  dec_frac_int /= div;
  return (frac){ .numerator =  dec_frac_int, .denominator = num };
}

int main() {
  int system;
  printf("Enter system:");
  scanf("%d", &system);
  double given_fraction;
  printf("Enter decimal fractions below to determine whether they are periodic or not:\n");
  while (scanf("%lf", &given_fraction) == 1) {
    frac object_fraction = to_fraction(given_fraction, EPSILON);
    switch (is_periodic(object_fraction, system)) {
      case HAS_END:
        printf("Fraction is NOT periodic!\n");
        break;
      case ENDLESS:
        printf("Fraction IS periodic!\n");
        break;
      case MEMORY_ALLOCATE_EXCEPTION:
        printf("Too few memory on your device :(\n");
        break;
    }
  }
  return 0;
}

#include "overflow.h"

int safe_sum(const int a, const int b, int* is_overflowed) {
  if ((a > 0 && b > 0 && INT_MAX - a < b) ||
      (a < 0 && b < 0 && INT_MIN - a > b)) {
        *is_overflowed = 1;
  } else {
    *is_overflowed = 0;
  }
  return a + b;
}

double safe_sum_dbl(const double a, const double b, int* is_overflowed) {
  if ((a > 0 && b > 0 && fabs(DBL_MAX - a - b) < DBL_EPSILON) ||
      (a < 0 && b < 0 && fabs(DBL_MIN - a - b) > DBL_EPSILON)) {
        *is_overflowed = 1;
  } else {
    *is_overflowed = 0;
  }
  return a + b;
}

//a - b
int safe_min(const int a, const int b, int* is_overflowed) {
  return safe_sum(a, -b, is_overflowed);
}

int safe_mult(const int a, const int b, int* is_overflowed) {
  if (!a || !b) {
    *is_overflowed = 0;
    return 0;
  }
  int multiple = a * b;
  if (multiple / b == a) {
    *is_overflowed = 0;
  } else {
    *is_overflowed = 1;
  }
  return multiple;
}

int safe_pow(const int a, const int b, int* is_overflowed) {
  int pow_result = 1;
  *is_overflowed = 0;
  for (int i = 0; i < b && !*is_overflowed; ++i) {
    pow_result = safe_mult(pow_result, a, is_overflowed);
  }
  return pow_result;
}

double safe_mult_dbl(const double a, const double b, int* is_overflowed) {
  if (fabs(a) < DBL_EPSILON || fabs(b) < DBL_EPSILON) {
    *is_overflowed = 0;
    return 0;
  }
  double multiple = a * b;
  if (fabs(multiple / b - a) < DBL_EPSILON) {
    *is_overflowed = 0;
  } else {
    *is_overflowed = 1;
  }
  return multiple;
}

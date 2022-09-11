#ifndef ___OVERFLOW___
#define ___OVERFLOW___

#include <limits.h>
#include <float.h>

int safe_sum(const int a, const int b, int* is_overflowed);
double safe_sum_dbl(const double a, const double b, int* is_overflowed);
int safe_min(const int a, const int b, int* is_overflowed);
int safe_mult(const int a, const int b, int* is_overflowed);
double safe_mult_dbl(const double a, const double b, int* is_overflowed);

#endif

#ifndef ___OVERFLOW___
#define ___OVERFLOW___

#include <limits.h>

unsigned int safe_sum(const unsigned int a, const unsigned int b, int* is_overflowed);
unsigned int safe_mult(const unsigned int a, const unsigned int b, int* is_overflowed);

#endif

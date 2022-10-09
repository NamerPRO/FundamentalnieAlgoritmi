#ifndef ___COVERFLOW___
#define ___COVERFLOW___

#include <limits.h>

unsigned int safe_sum(const unsigned int a, const unsigned int b, int* is_overflowed);
unsigned int safe_mult(const unsigned int a, const unsigned int b, int* is_overflowed);

#endif

#ifndef ___CIO___
#define ___CIO___

#include <stdarg.h>
#include <stdio.h>

#include "cstring.h"
#include "cinteger.h"
#include "cconstants.h"

int safe_read(FILE* stream, unsigned long int* count, char* pattern, int (*cmp)(int x), ...);

#endif

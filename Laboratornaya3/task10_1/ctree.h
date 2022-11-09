#ifndef ___CTREE___
#define ___CTREE___

#include "cstring.h"

typedef struct ctree {
  struct ctree** children;
  int children_count;
  string* value;
} ctree;

#endif

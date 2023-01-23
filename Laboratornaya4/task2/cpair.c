#include "cpair.h"

int pair_comporator(void* lhs, void* rhs, int* execute_status) {
  return !strcmp(((pair*)lhs)->name, ((pair*)rhs)->name);
}

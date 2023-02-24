#ifndef ___CPAIR___
#define ___CPAIR___

typedef struct pair {
  char* name;
  int power;
} pair;

int pair_comporator(void* lhs, void* rhs, int* execute_status);

#endif

#ifndef ___CMONOMIAL___
#define ___CMONOMIAL___

#include <stdlib.h>

#include "cconstants.h"
#include "hash_table.h"
#include "cnumber.h"
#include "cstring.h"
#include "cpair.h"

#define STANDART_MONOMIAL_SIZE 10

typedef struct monomial {
  int coefficient;
  int data_size;
  int struct_size;
  pair** unknowns;
} monomial;

monomial* create_monomial();
int monomial_comporator(void* lhs, void* rhs, int* execute_status);
int standart_monomial_comporator(const pair** lhs, const pair** rhs);
monomial* parse_monomial(string* monomial_element, int* is_free_term);
void print_monomial(monomial* element, int new_line);
void free_monomial(monomial* element);
int standart_monomial_collector(int symbol, int index);

#endif

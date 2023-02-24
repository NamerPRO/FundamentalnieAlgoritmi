#ifndef ___CPOLYNOMIAL___
#define ___CPOLYNOMIAL___

#include <stdlib.h>

#include "clist.h"
#include "cstring.h"
#include "cnumber.h"
#include "cmonomial.h"
#include "cconstants.h"
#include "hash_table_polynomial.h"

typedef struct polynomial {
  list* monomials;
  int free_term;
} polynomial;

polynomial* create_polynomial();
int copy_polynomial(polynomial* first, polynomial* second);
void standart_polynomial_clear_function(list_node* elem);
void free_polynomial(polynomial* element);
void monomial_list_clear(list_node* elem);
polynomial* parse_polynomial(string* polynomial_element);
void print_polynomial(polynomial* element);

#endif

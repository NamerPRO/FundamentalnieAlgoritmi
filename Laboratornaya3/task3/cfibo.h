#ifndef ___CFIBO___
#define ___CFIBO___

#include <stdlib.h>
#include <stdarg.h>

#include "clist.h"
#include "cfibo_struct.h"
#include "cconstants.h"

#define STANDART_FIBO_BROTHERS_SIZE 10

int create_fibo(fibo* fb);
unsigned long int fibo_size(fibo* fb);
int fibo_empty(fibo* fb);
int standart_fibo_node_comporator(FIBO_T lhs, FIBO_T rhs);
void fibo_unite(list* lhs, list* rhs);
fibo* fibo_melt(fibo* fb1, fibo* fb2, int (*cmp)(FIBO_T lhs, FIBO_T rhs));
void standart_fibo_clear(list_node* elem);
int fibo_remove_minimum(fibo* fb, int (*cmp)(FIBO_T lhs, FIBO_T rhs));
FIBO_T fibo_get_minimum(fibo* fb);
int fibo_insert(fibo* fb, FIBO_T elem, int (*cmp)(FIBO_T lhs, FIBO_T rhs));
int maximum_fibo_node_comporator(FIBO_T lhs, FIBO_T rhs);

#endif

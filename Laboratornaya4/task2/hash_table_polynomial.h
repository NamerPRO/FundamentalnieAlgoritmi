#ifndef ___HASH_TABLE_POLYNOMIAL___
#define ___HASH_TABLE_POLYNOMIAL___

#include <stdlib.h>
#include <stdarg.h>

#include "cconstants.h"
#include "cpair.h"
#include "clist.h"
#include "cstring.h"

#define HASH_PRIME_NUMBER 31
#define HASH_TABLE_POLYNOMIAL_SIZE 65537
#define HASH_TABLE_POLYNOMIAL_T monomial*

typedef struct hash_table_polynomial_node {
  HASH_TABLE_POLYNOMIAL_T value;
  struct hash_table_polynomial_node* next;
  struct hash_table_polynomial_node* past;
} hash_table_polynomial_node;

typedef struct hash_table_polynomial {
  hash_table_polynomial_node* head;
  unsigned long int size;
} hash_table_polynomial;

hash_table_polynomial* create_hash_table_polynomial(int size);
void create_hash_table_polynomial_line(hash_table_polynomial* hst);
unsigned long int hash_table_polynomial_size(hash_table_polynomial* lst);
int hash_table_polynomial_empty(hash_table_polynomial* lst);
int hash_table_polynomial_insert(hash_table_polynomial* lst, char* pattern, HASH_TABLE_POLYNOMIAL_T elem, ...);
int hash_table_polynomial_push_front(hash_table_polynomial* lst, HASH_TABLE_POLYNOMIAL_T elem);
int hash_table_polynomial_push_back(hash_table_polynomial* lst, HASH_TABLE_POLYNOMIAL_T elem);
void standart_hash_table_polynomial_clear(hash_table_polynomial_node* elem);
int hash_table_polynomial_remove(hash_table_polynomial* lst, char* pattern, void (*clear)(hash_table_polynomial_node* elem), ...);
int hash_table_polynomial_remove_front(hash_table_polynomial* lst, void (*clear)(hash_table_polynomial_node* elem));
int hash_table_polynomial_remove_back(hash_table_polynomial* lst, void (*clear)(hash_table_polynomial_node* elem));
HASH_TABLE_POLYNOMIAL_T hash_table_polynomial_extract_element(hash_table_polynomial* lst, char* pattern, int* execute_status, ...);
int hash_table_polynomial_free(hash_table_polynomial* lst, void (*clear)(hash_table_polynomial_node* elem));
hash_table_polynomial_node* hash_table_polynomial_extract_pointer(hash_table_polynomial* lst, char* pattern, int* execute_status, ...);
void hash_table_polynomial_put(hash_table_polynomial* hst, int hash, HASH_TABLE_POLYNOMIAL_T element);
HASH_TABLE_POLYNOMIAL_T hash_table_polynomial_get(hash_table_polynomial* table, int hash, int (*cmp)(void* lhs, void* rhs), HASH_TABLE_POLYNOMIAL_T rhs);
int get_monomial_hash(monomial* element);

#endif

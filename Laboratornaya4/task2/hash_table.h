#ifndef ___HASH_TABLE___
#define ___HASH_TABLE___

#include <stdlib.h>
#include <stdarg.h>

#include "cconstants.h"
#include "cpair.h"
#include "cstring.h"

#define HASH_PRIME_NUMBER 31
#define HASH_TABLE_SIZE 65537
#define HASH_TABLE_T pair*

typedef struct hash_table_node {
  HASH_TABLE_T value;
  struct hash_table_node* next;
  struct hash_table_node* past;
} hash_table_node;

typedef struct hash_table {
  hash_table_node* head;
  unsigned long int size;
} hash_table;

hash_table* create_hash_table(int size);
void create_hash_table_line(hash_table* hst);
unsigned long int hash_table_size(hash_table* lst);
int hash_table_empty(hash_table* lst);
int hash_table_insert(hash_table* lst, char* pattern, HASH_TABLE_T elem, ...);
int hash_table_push_front(hash_table* lst, HASH_TABLE_T elem);
int hash_table_push_back(hash_table* lst, HASH_TABLE_T elem);
void standart_hash_table_clear(hash_table_node* elem);
int hash_table_remove(hash_table* lst, char* pattern, void (*clear)(hash_table_node* elem), ...);
int hash_table_remove_front(hash_table* lst, void (*clear)(hash_table_node* elem));
int hash_table_remove_back(hash_table* lst, void (*clear)(hash_table_node* elem));
HASH_TABLE_T hash_table_extract_element(hash_table* lst, char* pattern, int* execute_status, ...);
int hash_table_free(hash_table* lst, void (*clear)(hash_table_node* elem));
hash_table_node* hash_table_extract_pointer(hash_table* lst, char* pattern, int* execute_status, ...);
void hash_table_put(hash_table* hst, int hash, HASH_TABLE_T element);
HASH_TABLE_T hash_table_get(hash_table* table, int hash, int (*cmp)(void* lhs, void* rhs), HASH_TABLE_T rhs);
int get_hash(char* c_str);

#endif

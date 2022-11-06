#ifndef ___CTRIE___
#define ___CTRIE___

#include "cbst.h"
#include "cstring.h"
#include "clist.h"

typedef struct trie_node {
  struct trie_node** trie_ptr;
  int trie_ptr_size;
  char symbol;
  bst_node* b_node;
} trie_node;

int create_trie(trie_node** t);
int trie_insert(trie_node* t, int i, string* s, bst_node** BST_ROOT);

#endif

#ifndef ___BST___
#define ___BST___

#include "cconstants.h"
#include "clist.h"

// typedef bst_node_data BST_TYPE;

typedef int BST_KEY;

typedef struct bst_node_data {
  list* words;
  BST_KEY count; //столько раз слово встречается в тексте
} BST_TYPE;

typedef struct bst_node {
  BST_TYPE* data;
  struct bst_node* left;
  struct bst_node* right;
} bst_node;

int create_bst(bst_node** b);
bst_node* bst_find(bst_node* b, BST_KEY key, int (*cmp)(BST_KEY lhs, BST_KEY rhs));
bst_node* bst_insert(bst_node* b, BST_KEY key, int* execute_status, int (*cmp)(BST_KEY lhs, BST_KEY rhs), bst_node** BST_ROOT);
int bst_remove(bst_node* b, BST_KEY key, void (*cls)(BST_TYPE* structure), int (*cmp)(BST_KEY lhs, BST_KEY rhs), bst_node** BST_ROOT);
int standart_bst_comporator(BST_KEY lhs, BST_KEY rhs);
void standart_bst_clear(BST_TYPE* structure);

#endif

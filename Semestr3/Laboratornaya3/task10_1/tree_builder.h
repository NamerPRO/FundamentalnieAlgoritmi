#ifndef ___TREE_BUILDER___
#define ___TREE_BUILDER___

#include <stdlib.h>
#include <ctype.h>

#include "cstring.h"
#include "cstack.h"
#include "ctree.h"

#define OPEN_BRACKET NULL

void create_tree(ctree* t);
int read_sep(int symbol);
int insert_tree_node_in_stack(stack* st, string* str);
int fill_array_from_stack(stack* st, ctree*** mchildren, int* i);
int build_tree(string* pattern, ctree* tree);
void print_tree(ctree* t, int h, FILE* stream);
void children_free_tree(ctree* t);
void free_tree(ctree* t);

#endif

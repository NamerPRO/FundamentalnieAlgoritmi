#ifndef ___STATISTICS___
#define ___STATISTICS___

#include "cstring.h"
#include "ctrie.h"

int count_occurrences(trie_node* t, int i, string* s);
void get_most_common_k_words(bst_node* b, list* lst, int* n, int* execute_status);

#endif

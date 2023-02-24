#include "statistics.h"

int count_occurrences(trie_node* t, int i, string* s) {
  if (i == (int)get_string_size(s)) {
    if (t->b_node == NULL) {
      return -1;
    }
    return t->b_node->data->count;
  }
  int l = 0;
  int r = t->trie_ptr_size - 1;
  char symbol = get_raw_string(s)[i];
  while (l <= r) {
    int mid = (l - r) / 2 + r;
    if (t->trie_ptr[mid]->symbol < symbol) {
      l = mid + 1;
    } else if (t->trie_ptr[mid]->symbol > symbol) {
      r = mid - 1;
    } else {
      return count_occurrences(t->trie_ptr[mid], i + 1, s);
    }
  }
  return -1;
}

void get_most_common_k_words(bst_node* b, list* lst, int* n, int* execute_status) {
  *execute_status = SUCCESS_FUNCTION_RETURN;
  if (b != NULL) {
    get_most_common_k_words(b->right, lst, n, execute_status);
    if (!(*n) || *execute_status != SUCCESS_FUNCTION_RETURN) {
      return;
    }
    int i = 0;
    while (*n && (i < (int)list_size(b->data->words))) {
      --(*n);
      LIST_TYPE element = list_element_at(b->data->words, i++, execute_status);
      if (*execute_status != SUCCESS_FUNCTION_RETURN) {
        return;
      }
      if ((*execute_status = list_push_front(lst, element)) != SUCCESS_FUNCTION_RETURN) {
        return;
      }
    }
    if (!(*n)) {
      return;
    }
    get_most_common_k_words(b->left, lst, n, execute_status);
  }
}

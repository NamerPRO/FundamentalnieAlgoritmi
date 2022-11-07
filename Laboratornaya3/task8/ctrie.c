#include "ctrie.h"

int create_trie(trie_node** t) {
  *t = (trie_node*)malloc(sizeof(trie_node));
  if (*t == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  (*t)->trie_ptr = NULL;
  (*t)->trie_ptr_size = 0;
  (*t)->symbol = 0;
  (*t)->b_node = NULL;
  return SUCCESS_FUNCTION_RETURN;
}

//BST_ONE_NODE -> указывает на ячейку с единицей
//BST_ROOT -> указывает на корень дерева всегда
int trie_insert(trie_node* t, int i, string* s, bst_node** BST_ROOT) {
  if (i == (int)get_string_size(s)) {
    //Слово уже вставлено!!!
    int execute_status;
    int to_find = (t->b_node == NULL) ? 1 : t->b_node->data->count + 1;
    bst_node* o = bst_find(*BST_ROOT, to_find, standart_bst_comporator);
    if (o != NULL) {
      //Такая вершина уже есть. Перестраивать дерево не надо
      execute_status = list_push_front(o->data->words, s);
      if (execute_status != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
    } else {
      //Придется перестроить дерево, добавив в него новую вершину
      o = bst_insert(*BST_ROOT, to_find, &execute_status, standart_bst_comporator, BST_ROOT);
      if (execute_status != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
      execute_status = list_push_front(o->data->words, s);
      if (execute_status != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
    }
    if (t->b_node != NULL) {
      execute_status = list_pop_by(t->b_node->data->words, s, clear_list_type);
      if (execute_status != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
      if (list_empty(t->b_node->data->words)) {
        execute_status = bst_remove(*BST_ROOT, t->b_node->data->count, standart_bst_clear, standart_bst_comporator, BST_ROOT);
        t->b_node = o;
        return execute_status;
      }
    }
    t->b_node = o;
    return SUCCESS_FUNCTION_RETURN;
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
      return trie_insert(t->trie_ptr[mid], i + 1, s, BST_ROOT);
    }
  }
  //Нашли где надо довставлять!
  //не забыть про перебалансировку trie_ptr,
  //по лексикографическому порядку

  //вставляем в позицию l

  trie_node** possitble_trie_ptr = (trie_node**)realloc(t->trie_ptr, sizeof(trie_node*) * (t->trie_ptr_size + 1));
  // trie_node** possitble_trie_ptr = (trie_node**)malloc(sizeof(trie_node*) * (t->trie_ptr_size + 1));
  if (possitble_trie_ptr == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  t->trie_ptr = possitble_trie_ptr;
  ++t->trie_ptr_size;
  for (int j = t->trie_ptr_size - 1; j > l; --j) {
    t->trie_ptr[j] = t->trie_ptr[j - 1];
  }
  t->trie_ptr[l] = (trie_node*)malloc(sizeof(trie_node));
  if (t->trie_ptr[l] == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  t->trie_ptr[l]->trie_ptr_size = 0;
  t->trie_ptr[l]->symbol = get_raw_string(s)[i];
  t->trie_ptr[l]->b_node = NULL;
  ++i;
  t = t->trie_ptr[l];
  for (; i < (int)get_string_size(s); ++i) {
    // trie_node** possitble_trie_ptr = (trie_node**)realloc(t->trie_ptr, sizeof(trie_node*));
    trie_node** possitble_trie_ptr = (trie_node**)malloc(sizeof(trie_node*));
    if (possitble_trie_ptr == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    t->trie_ptr = possitble_trie_ptr;
    ++t->trie_ptr_size;
    t->trie_ptr[0] = (trie_node*)malloc(sizeof(trie_node));
    if (t->trie_ptr[0] == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    t->trie_ptr[0]->trie_ptr_size = 0;
    t->trie_ptr[0]->symbol = get_raw_string(s)[i];
    t->trie_ptr[0]->b_node = NULL;
    t = t->trie_ptr[0];
  }
  //t - это указать на конец слова.
  //надо поставить его соответств эл-т дерева

  int execute_status;
  bst_node* o = bst_find(*BST_ROOT, 1, standart_bst_comporator);
  if (o != NULL) {
    //Такая вершина уже есть. Перестраивать дерево не надо
    execute_status = list_push_front(o->data->words, s);
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  } else {
    //Придется перестроить дерево, добавив в него новую вершину
    o = bst_insert(*BST_ROOT, 1, &execute_status, standart_bst_comporator, BST_ROOT);
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    execute_status = list_push_front(o->data->words, s);
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  t->b_node = o;
  t->trie_ptr = NULL;
  return SUCCESS_FUNCTION_RETURN;
}

void trie_destroy(trie_node* t) {
  if (t->trie_ptr != NULL) {
    for (int i = 0; i < t->trie_ptr_size; ++i) {
      trie_destroy(t->trie_ptr[i]);
    }
    free(t->trie_ptr);
  }
  free(t);
}

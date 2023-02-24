#include "hash_table_polynomial.h"

hash_table_polynomial* create_hash_table_polynomial(int size) {
  hash_table_polynomial* table = (hash_table_polynomial*)malloc(sizeof(hash_table_polynomial) * size);
  if (table == NULL) {
    return NULL;
  }
  for (int i = 0; i < size; ++i) {
    create_hash_table_polynomial_line(table + i);
  }
  return table;
}

void create_hash_table_polynomial_line(hash_table_polynomial* hst) {
  hst->head = NULL;
  hst->size = 0;
}

void hash_table_polynomial_put(hash_table_polynomial* hst, int hash, HASH_TABLE_POLYNOMIAL_T element) {
  hash_table_polynomial_push_back(hst + hash, element);
}

unsigned long int hash_table_polynomial_size(hash_table_polynomial* hst) {
  return hst->size;
}

int hash_table_polynomial_empty(hash_table_polynomial* hst) {
  return !hst->size;
}

int hash_table_polynomial_insert(hash_table_polynomial* hst, char* pattern, HASH_TABLE_POLYNOMIAL_T elem, ...) {
  if (hash_table_polynomial_empty(hst)) {
    hst->head = (hash_table_polynomial_node*)malloc(sizeof(hash_table_polynomial_node));
    if (hst->head == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    hst->head->value = elem;
    hst->head->next = hst->head;
    hst->head->past = hst->head;
    hst->size = 1;
    return SUCCESS_FUNCTION_RETURN;
  }
  hash_table_polynomial_node* hst_node = (hash_table_polynomial_node*)malloc(sizeof(hash_table_polynomial_node));
  if (hst_node == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  hst_node->value = elem;
  va_list arg;
  va_start(arg, elem);
  hash_table_polynomial_node* hst_ptr = hst->head;
  if (pattern[1] == 'd') {
    unsigned long int index = va_arg(arg, unsigned long int);
    if (index > hst->size) {
      return INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (!index) {
      hst_ptr = hst_ptr->past;
      hst->head = hst_node;
    } else if (index == hst->size) {
      hst_ptr = hst->head->past;
    } else {
      for (unsigned long int i = 0; i < index - 1; ++i) {
        hst_ptr = hst_ptr->next;
      }
    }
  } else {
    //вставляет вправо, в то время,
    //как вставляя по индексу мы вставим влево
    hst_ptr = va_arg(arg, hash_table_polynomial_node*);
    // hst_ptr = hst_ptr->past;
  }
  va_end(arg);
  hash_table_polynomial_node* hst_save = hst_ptr->next;
  hst_ptr->next = hst_node;
  hst_node->past = hst_ptr;
  hst_node->next = hst_save;
  hst_save->past = hst_node;
  ++hst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int hash_table_polynomial_push_front(hash_table_polynomial* hst, HASH_TABLE_POLYNOMIAL_T elem) {
  return hash_table_polynomial_insert(hst, "%d", elem, 0);
}

int hash_table_polynomial_push_back(hash_table_polynomial* hst, HASH_TABLE_POLYNOMIAL_T elem) {
  return hash_table_polynomial_insert(hst, "%d", elem, hst->size);
}

void standart_hash_table_polynomial_clear(hash_table_polynomial_node* elem) {
  free(elem);
}

int hash_table_polynomial_remove(hash_table_polynomial* hst, char* pattern, void (*clear)(hash_table_polynomial_node* elem), ...) {
  va_list arg;
  va_start(arg, clear);
  if (hash_table_polynomial_empty(hst)) {
    return INDEX_OUT_OF_BOUNDS_EXCEPTION;
  }
  hash_table_polynomial_node* hst_ptr = hst->head;
  if (pattern[1] == 'd') {
    unsigned long int index = va_arg(arg, unsigned long int);
    if (index >= hst->size) {
      return INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (!index) {
      hst_ptr = hst_ptr->past;
      hst->head = hst->head->next;
    } else if (index == hst->size) {
      hst_ptr = hst->head->past->past;
    } else {
      for (unsigned long int i = 0; i < index - 1; ++i) {
        hst_ptr = hst_ptr->next;
      }
    }
  } else {
    hst_ptr = va_arg(arg, hash_table_polynomial_node*);
    if (hst_ptr == hst->head) {
      hst->head = hst->head->next;
    }
    hst_ptr = hst_ptr->past;
  }
  va_end(arg);
  hash_table_polynomial_node* hst_save = hst_ptr->next;
  hst_ptr->next->next->past = hst_ptr;
  hst_ptr->next = hst_ptr->next->next;
  clear(hst_save);
  --hst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int hash_table_polynomial_remove_front(hash_table_polynomial* hst, void (*clear)(hash_table_polynomial_node* elem)) {
  return hash_table_polynomial_remove(hst, "%d", clear, 0);
}

int hash_table_polynomial_remove_back(hash_table_polynomial* hst, void (*clear)(hash_table_polynomial_node* elem)) {
  return hash_table_polynomial_remove(hst, "%d", clear, hst->size - 1);
}

int hash_table_polynomial_free(hash_table_polynomial* hst, void (*clear)(hash_table_polynomial_node* elem)) {
  int execute_status;
  while (!hash_table_polynomial_empty(hst)) {
    if ((execute_status = hash_table_polynomial_remove_front(hst, clear)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

// int get_hash(char* c_str) {
//   int hash = 0;
//   int p_coefficient = 1;
//   for (int i = 0; c_str[i]; ++i) {
//     hash = (hash + (cin(c_str[i], '0', '9', GG) ? c_str[i] - '0' : cin(c_str[i], 'a', 'z', GG) ? c_str[i] - 'a' + 10 : c_str[i]) * p_coefficient) % HASH_TABLE_POLYNOMIAL_SIZE;
//     p_coefficient *= HASH_PRIME_NUMBER;
//   }
//   return hash;
// }

int get_monomial_hash(monomial* element) {
  int hash = 0;
  int p_coefficient = 1;
  for (int i = 0; i < element->data_size; ++i) {
    hash = (hash + get_hash(element->unknowns[i]->name) % HASH_TABLE_POLYNOMIAL_SIZE + ((get_hash(element->unknowns[i]->name)  % HASH_TABLE_POLYNOMIAL_SIZE) * p_coefficient)  % HASH_TABLE_POLYNOMIAL_SIZE) % HASH_TABLE_POLYNOMIAL_SIZE;
    p_coefficient *= HASH_PRIME_NUMBER;
  }
}

HASH_TABLE_POLYNOMIAL_T hash_table_polynomial_get(hash_table_polynomial* table, int hash, int (*cmp)(void* lhs, void* rhs), HASH_TABLE_POLYNOMIAL_T rhs) {
  int execute_status;
  hash_table_polynomial* table_line = table + hash;
  if (hash_table_polynomial_empty(table_line)) {
    return NULL;
  } else {
    hash_table_polynomial_node* table_element = table_line->head;
    int table_size = hash_table_polynomial_size(table_line);
    for (int i = 0; i < table_size; ++i) {
      HASH_TABLE_POLYNOMIAL_T possible_pair = hash_table_polynomial_extract_element(table_line, "%p", &execute_status, table_element);
      table_element = table_element->next;
      if (cmp(possible_pair, rhs)) {
        return possible_pair;
      }
    }
  }
  return NULL;
}

HASH_TABLE_POLYNOMIAL_T hash_table_polynomial_extract_element(hash_table_polynomial* hst, char* pattern, int* execute_status, ...) {
  if (hash_table_polynomial_empty(hst)) {
    *execute_status = INDEX_OUT_OF_BOUNDS_EXCEPTION;
    return 0;
  }
  va_list arg;
  va_start(arg, execute_status);
  hash_table_polynomial_node* hst_ptr = hst->head;
  if (pattern[1] == 'd') {
    unsigned long int index = va_arg(arg, unsigned long int);
    if (index >= hst->size) {
      *execute_status = INDEX_OUT_OF_BOUNDS_EXCEPTION;
      return 0;
    }
    for (unsigned long int i = 0; i < index; ++i) {
      hst_ptr = hst_ptr->next;
    }
  } else {
    hst_ptr = va_arg(arg, hash_table_polynomial_node*);
  }
  va_end(arg);
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return hst_ptr->value;
}

hash_table_polynomial_node* hash_table_polynomial_extract_pointer(hash_table_polynomial* hst, char* pattern, int* execute_status, ...) {
  if (hash_table_polynomial_empty(hst)) {
    *execute_status = INDEX_OUT_OF_BOUNDS_EXCEPTION;
    return 0;
  }
  va_list arg;
  va_start(arg, execute_status);
  hash_table_polynomial_node* hst_ptr = hst->head;
  if (pattern[1] == 'd') {
    unsigned long int index = va_arg(arg, unsigned long int);
    if (index >= hst->size) {
      *execute_status = INDEX_OUT_OF_BOUNDS_EXCEPTION;
      return 0;
    }
    for (unsigned long int i = 0; i < index; ++i) {
      hst_ptr = hst_ptr->next;
    }
  } else {
    hst_ptr = va_arg(arg, hash_table_polynomial_node*);
  }
  va_end(arg);
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return hst_ptr;
}

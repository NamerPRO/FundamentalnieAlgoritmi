#include "cpolynomial.h"

int copy_polynomial(polynomial* first, polynomial* second) {
  list_node* node = second->monomials->head;
  for (int i = 0; i < second->monomials->size; ++i) {
    // monomial* mono = create_monomial();
    // if (mono == NULL) {
    //   //...
    //   return MEMORY_ALLOCATE_EXCEPTION;
    // }
    monomial* mono = (monomial*)malloc(sizeof(monomial));
    if (mono == NULL) {
      return NULL;
    }
    mono->data_size = node->value->data_size;
    mono->struct_size = node->value->struct_size;
    mono->coefficient = node->value->coefficient;
    mono->unknowns = (pair**)malloc(sizeof(pair*) * node->value->struct_size);
    if (mono->unknowns == NULL) {
      //...
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    for (int j = 0; j < mono->data_size; ++j) {
      mono->unknowns[j] = (pair*)malloc(sizeof(pair));
      if (mono->unknowns[j] == NULL) {
        //...
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      mono->unknowns[j]->power = node->value->unknowns[j]->power;
      mono->unknowns[j]->name = (char*)malloc(sizeof(char) * strlen(node->value->unknowns[j]->name));
      if (mono->unknowns[j]->name == NULL) {
        //...
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      strcpy(mono->unknowns[j]->name, node->value->unknowns[j]->name);
    }
    list_push_back(first->monomials, mono);
    node = node->next;
  }
  first->free_term = second->free_term;
  return SUCCESS_FUNCTION_RETURN;
}

polynomial* create_polynomial() {
  polynomial* created_polynomial = (polynomial*)malloc(sizeof(polynomial));
  if (created_polynomial == NULL) {
    return NULL;
  }
  created_polynomial->monomials = (list*)malloc(sizeof(list));
  if (created_polynomial->monomials == NULL) {
    free(created_polynomial);
    return NULL;
  }
  create_list(created_polynomial->monomials);
  created_polynomial->free_term = 0;
  return created_polynomial;
}

void standart_polynomial_clear_function(list_node* elem) {
  free_monomial(elem->value);
  free(elem);
}

void free_polynomial(polynomial* element) {
  list_free(element->monomials, standart_polynomial_clear_function);
  free(element->monomials);
  free(element);
}

void monomial_list_clear(list_node* elem) {
  free_monomial(elem->value);
  free(elem);
}

polynomial* parse_polynomial(string* polynomial_element) {
  int is_free_term;
  unsigned long int i = 0;
  polynomial* created_polynomial = create_polynomial();
  if (created_polynomial == NULL) {
    return NULL;
  }
  created_polynomial->free_term = 0;
  string* monomial_element = heap_string(EMPTY);
  if (monomial_element == NULL) {
    free_polynomial(created_polynomial);
    return NULL;
  }
  hash_table_polynomial* client_table = create_hash_table_polynomial(HASH_TABLE_SIZE);
  if (client_table == NULL) {
    free_polynomial(created_polynomial);
    free_heap_string(monomial_element);
    return NULL;
  }
  char previous_operation = get_raw_string(polynomial_element)[0];
  if (previous_operation == '-') {
    i = 1;
  }
  do {
    if (i > 0) {
      previous_operation = get_raw_string(polynomial_element)[i - 1];
    }
    if (read_string(FROM_STRING, polynomial_element, standart_monomial_collector, monomial_element, &i) != SUCCESS_FUNCTION_RETURN) {
      free_polynomial(created_polynomial);
      free_heap_string(monomial_element);
      for (int i = 0; i < HASH_TABLE_POLYNOMIAL_SIZE; ++i) {
        hash_table_polynomial_free(client_table + i, standart_hash_table_clear);
      }
      free(client_table);
      return NULL;
    }
    monomial* created_monomial = parse_monomial(monomial_element, &is_free_term);
    if (created_monomial == NULL) {
      free_polynomial(created_polynomial);
      free_heap_string(monomial_element);
      for (int i = 0; i < HASH_TABLE_POLYNOMIAL_SIZE; ++i) {
        hash_table_polynomial_free(client_table + i, standart_hash_table_clear);
      }
      free(client_table);
      return NULL;
    }
    if (!created_monomial->coefficient) {
      free_monomial(created_monomial);
    } else {
      if (previous_operation == '-') {
        created_monomial->coefficient *= -1;
      }
      if (is_free_term) {
        created_polynomial->free_term += created_monomial->coefficient;
        free_monomial(created_monomial);
      } else {
        int monomial_hash = get_monomial_hash(created_monomial);
        monomial* table_element = hash_table_polynomial_get(client_table, monomial_hash, monomial_comporator, created_monomial);
        if (table_element == NULL) {
          list_push_back(created_polynomial->monomials, created_monomial);
          hash_table_polynomial_put(client_table, monomial_hash, created_monomial);
        } else {
          table_element->coefficient += created_monomial->coefficient;
          free_monomial(created_monomial);
        }
      }
    }
  } while(sat(polynomial_element, i - 1));
  list_node* monomial_node = created_polynomial->monomials->head;
  for (int i = 0; i < list_size(created_polynomial->monomials); ++i) {
    if (monomial_node->value->coefficient == 0) {
      list_remove(created_polynomial->monomials, "%p", monomial_list_clear, monomial_node);
    }
    monomial_node = monomial_node->next;
  }
  free_heap_string(monomial_element);
  for (int i = 0; i < HASH_TABLE_POLYNOMIAL_SIZE; ++i) {
    hash_table_polynomial_free(client_table + i, standart_hash_table_clear);
  }
  free(client_table);
  return created_polynomial;
}

void print_polynomial(polynomial* element) {
  list_node* monomial_node = element->monomials->head;
  int flag = 1;
  for (int i = 0; i < element->monomials->size; ++i) {
    flag = 0;
    if (monomial_node->value->coefficient > 0 && i) {
      printf("+");
    }
    print_monomial(monomial_node->value, 0);
    monomial_node = monomial_node->next;
  }
  if (element->free_term != 0 || flag) {
    if (element->free_term > 0 && element->monomials->size) {
      printf("+");
    }
    printf("%d", element->free_term);
  }
  printf("\n");
}

#include "cpolymath.h"

polynomial* get_memoried_polynomial(int id) {
  return response_storage[id];
}

void save_polynomial(int id, polynomial* poly) {
  response_storage[id] = poly;
}

void drop_memoried_polynomial(int id) {
  if (get_memoried_polynomial(id) != NULL) {
    free_polynomial(response_storage[id]);
    response_storage[id] = NULL;
  }
}

int add(void* data) {
  polynomial* first = ((struct thrd_dt*)data)->poly1;
  polynomial* second = ((struct thrd_dt*)data)->second.poly2;
  int id = ((struct thrd_dt*)data)->id;
  if (first == NULL) {
    first = get_memoried_polynomial(id);
  } else {
    drop_memoried_polynomial(id);
  }
  if (first == NULL) {
    save_polynomial(id, second);
    is_result_new[id] = 1;
    return;
  }
  first->free_term += second->free_term;
  hash_table_polynomial* server_table = create_hash_table_polynomial(HASH_TABLE_SIZE);
  if (server_table == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  list_node* mono_node = first->monomials->head;
  for (int i = 0; i < first->monomials->size; ++i) {
    hash_table_polynomial_put(server_table, get_monomial_hash(mono_node->value), mono_node->value);
    mono_node = mono_node->next;
  }
  mono_node = second->monomials->head;
  for (int i = 0; i < second->monomials->size; ++i) {
    monomial* table_element = hash_table_polynomial_get(server_table, get_monomial_hash(mono_node->value), monomial_comporator, mono_node->value);
    if (table_element == NULL) {
      list_push_back(first->monomials, mono_node->value);
      hash_table_polynomial_put(server_table, get_monomial_hash(mono_node->value), mono_node->value);
    } else {
      table_element->coefficient += mono_node->value->coefficient;
      free_monomial(mono_node->value);
    }
    mono_node = mono_node->next;
  }
  mono_node = first->monomials->head;
  int sz = first->monomials->size;
  for (int i = 0; i < sz; ++i) {
    if (mono_node->value->coefficient == 0) {
      free_monomial(mono_node->value);
      list_node* node_save = mono_node;
      mono_node = mono_node->next;
      list_remove(first->monomials, "%p", standart_list_clear, node_save);
    } else {
      mono_node = mono_node->next;
    }
    if (mono_node == NULL) {
      mono_node = first->monomials->head;
    }
  }
  for (int i = 0; i < HASH_TABLE_POLYNOMIAL_SIZE; ++i) {
    hash_table_polynomial_free(server_table + i, standart_hash_table_clear);
  }
  free(server_table);
  list_free(second->monomials, standart_list_clear);
  free(second);
  save_polynomial(id, first);
  is_result_new[id] = 1;
  return SUCCESS_FUNCTION_RETURN;
}

int sub(void* data) {
  polynomial* first = ((struct thrd_dt*)data)->poly1;
  polynomial* second = ((struct thrd_dt*)data)->second.poly2;
  int id = ((struct thrd_dt*)data)->id;
  if (second == NULL) {
    second = get_memoried_polynomial(id);
    if (second == NULL) {
      list_node* node = first->monomials->head;
      for (int i = 0; i < first->monomials->size; ++i) {
        node->value->coefficient *= -1;
        node = node->next;
      }
      first->free_term *= -1;
      save_polynomial(id, first);
      is_result_new[id] = 1;
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    polynomial* c = first;
    first = second;
    second = c;
  } else {
    drop_memoried_polynomial(id);
  }
  first->free_term -= second->free_term;
  hash_table_polynomial* server_table = create_hash_table_polynomial(HASH_TABLE_SIZE);
  if (server_table == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  list_node* mono_node = first->monomials->head;
  for (int i = 0; i < first->monomials->size; ++i) {
    hash_table_polynomial_put(server_table, get_monomial_hash(mono_node->value), mono_node->value);
    mono_node = mono_node->next;
  }
  mono_node = second->monomials->head;
  for (int i = 0; i < second->monomials->size; ++i) {
    monomial* table_element = hash_table_polynomial_get(server_table, get_monomial_hash(mono_node->value), monomial_comporator, mono_node->value);
    if (table_element == NULL) {
      mono_node->value->coefficient *= -1;
      list_push_back(first->monomials, mono_node->value);
      hash_table_polynomial_put(server_table, get_monomial_hash(mono_node->value), mono_node->value);
    } else {
      table_element->coefficient -= mono_node->value->coefficient;
      free_monomial(mono_node->value);
    }
    mono_node = mono_node->next;
  }
  mono_node = first->monomials->head;
  int sz = first->monomials->size;
  for (int i = 0; i < sz; ++i) {
    if (mono_node->value->coefficient == 0) {
      free_monomial(mono_node->value);
      list_node* node_save = mono_node;
      mono_node = mono_node->next;
      list_remove(first->monomials, "%p", standart_list_clear, node_save);
    } else {
      mono_node = mono_node->next;
    }
    if (mono_node == NULL) {
      mono_node = first->monomials->head;
    }
  }
  list_free(second->monomials, standart_list_clear);
  free(second);
  save_polynomial(id, first);
  is_result_new[id] = 1;
  for (int i = 0; i < HASH_TABLE_POLYNOMIAL_SIZE; ++i) {
    hash_table_polynomial_free(server_table + i, standart_hash_table_clear);
  }
  free(server_table);
  return SUCCESS_FUNCTION_RETURN;
}

int mult_monomials(monomial* first, monomial* second) {
  for (int i = 0; i < first->data_size; ++i) {
    pair* unknown = first->unknowns[i];
    int left = 0;
    int right = second->data_size - 1;
    int flag = 0;
    while (left <= right) {
      int mid = (right - left) / 2 + left;
      int cmp_res = strcmp(unknown->name, second->unknowns[mid]->name);
      if (cmp_res < 0) {
        right = mid - 1;
      } else if (cmp_res > 0) {
        left = mid + 1;
      } else {
        flag = 1;
        second->unknowns[mid]->power += unknown->power;
        break;
      }
    }
    if (!flag) {
      pair** possible_unknowns = NULL;
      if (second->data_size == second->struct_size) {
        possible_unknowns = (pair**)realloc(second->unknowns, sizeof(pair*) * second->struct_size * 2);
        if (possible_unknowns == NULL) {
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        second->unknowns = possible_unknowns;
        second->struct_size *= 2;
      }
      pair* unknown_cpy = (pair*)malloc(sizeof(pair));
      if (unknown_cpy == NULL) {
        free(possible_unknowns);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      unknown_cpy->name = (char*)malloc(sizeof(char) * strlen(unknown->name));
      if (unknown_cpy->name == NULL) {
        free(possible_unknowns);
        free(unknown_cpy->name);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      strcpy(unknown_cpy->name, unknown->name);
      unknown_cpy->power = unknown->power;
      second->unknowns[second->data_size++] = unknown_cpy;
      qsort(second->unknowns, second->data_size, sizeof(pair*), standart_monomial_comporator);
    }
  }
  second->coefficient *= first->coefficient;
  return SUCCESS_FUNCTION_RETURN;
}

int mult(void* data) {
  polynomial* first = ((struct thrd_dt*)data)->poly1;
  polynomial* second = ((struct thrd_dt*)data)->second.poly2;
  int id = ((struct thrd_dt*)data)->id;
  if (first == NULL) {
    first = get_memoried_polynomial(id);
    save_polynomial(id, NULL);
    if (first == NULL) {
      polynomial* poly = create_polynomial();
      if (poly == NULL) {
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      save_polynomial(id, poly);
      is_result_new[id] = 1;
      return SUCCESS_FUNCTION_RETURN;
    }
  } else {
    drop_memoried_polynomial(id);
  }
  int new_free_term = first->free_term * second->free_term;
  if (first->monomials->size < second->monomials->size) {
    polynomial* c = first;
    first = second;
    second = c;
  }
  list_node* mono_first = first->monomials->head;
  int new_mono_size = first->monomials->size + !!first->free_term + !!second->free_term;
  polynomial** poly_second = (polynomial**)malloc(sizeof(polynomial*) * new_mono_size);
  if (poly_second == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (int i = 0; i < new_mono_size; ++i) {
    poly_second[i] = create_polynomial();
    if (poly_second[i] == NULL) {
      for (int j = 0; j < i; ++j) {
        free_polynomial(poly_second[j]);
      }
      free(poly_second);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if (copy_polynomial(poly_second[i], second) == MEMORY_ALLOCATE_EXCEPTION) {
      for (int j = 0; j < i; ++j) {
        free_polynomial(poly_second[j]);
      }
      free(poly_second);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    poly_second[i]->free_term = 0;
  }
  list_node* mono_second;
  int i = 0;
  for (; i < first->monomials->size; ++i) {
    mono_second = poly_second[i]->monomials->head;
    for (int j = 0; j < second->monomials->size; ++j) {
      mult_monomials(mono_first->value, mono_second->value);
      mono_second = mono_second->next;
    }
    mono_first = mono_first->next;
  }
  if (first->free_term) {
    mono_second = poly_second[i++]->monomials->head;
    for (int j = 0; j < second->monomials->size; ++j) {
      mono_second->value->coefficient *= first->free_term;
      mono_second = mono_second->next;
    }
  }

  if (second->free_term) {
    poly_second[i] = create_polynomial();
    if (poly_second[i] == NULL) {
      for (int j = 0; j < i; ++j) {
        free_polynomial(poly_second[j]);
      }
      free(poly_second);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if (copy_polynomial(poly_second[i], first) == MEMORY_ALLOCATE_EXCEPTION) {
      for (int j = 0; j < i; ++j) {
        free_polynomial(poly_second[j]);
      }
      free(poly_second);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    poly_second[i]->free_term = 0;
    mono_second = poly_second[i]->monomials->head;
    for (int j = 0; j < first->monomials->size; ++j) {
      mono_second->value->coefficient *= second->free_term;
      mono_second = mono_second->next;
    }
  }
  if (new_mono_size <= 1) {
    save_polynomial(id, poly_second[0]);
  } else {
    struct thrd_dt dt = { .poly1 = poly_second[0], .second.poly2 = poly_second[1], .id = id };
    add(&dt);
    for (int i = 2; i < new_mono_size; ++i) {
      struct thrd_dt dt = { .poly1 = NULL, .second.poly2 = poly_second[i], .id = id };
      add(&dt);
    }
  }
  get_memoried_polynomial(id)->free_term = new_free_term;
  is_result_new[id] = 1;
  return SUCCESS_FUNCTION_RETURN;
}

int diff(void* data) {
  polynomial* first = ((struct thrd_dt*)data)->poly1;
  char* variable = ((struct thrd_dt*)data)->second.var;
  int id = ((struct thrd_dt*)data)->id;
  if (first == NULL) {
    first = get_memoried_polynomial(id);
    if (first == NULL) {
      return ARGUMENTS_EXCEPTION;
    }
  } else {
    drop_memoried_polynomial(id);
  }
  first->free_term = 0;
  list_node* node = first->monomials->head;
  for (int i = 0; i < first->monomials->size; ++i) {
    int flag = 0;
    for (int j = 0; j < node->value->data_size; ++j) {
      if (!strcmp(node->value->unknowns[j]->name, variable)) {
        node->value->coefficient *= node->value->unknowns[j]->power;
        node->value->unknowns[j]->power -= 1;
        if (node->value->unknowns[j]->power == 0) {
          free(node->value->unknowns[j]->name);
          free(node->value->unknowns[j]);
          for (int k = j + 1; k < node->value->data_size; ++k) {
            node->value->unknowns[k - 1] = node->value->unknowns[k];
          }
          node->value->data_size -= 1;
        }
        flag = 1;
        if (node->value->data_size == 0) {
          first->free_term += node->value->coefficient;
          flag = 0;
        }
        break;
      }
    }
    if (!flag) {
      node->value->coefficient = 0;
    }
    node = node->next;
  }
  node = first->monomials->head;
  int sz = first->monomials->size;
  for (int i = 0; i < sz; ++i) {
    if (node->value->coefficient == 0) {
      free_monomial(node->value);
      list_node* node_save = node;
      node = node->next;
      list_remove(first->monomials, "%p", standart_list_clear, node_save);
    } else {
      node = node->next;
    }
    if (node == NULL) {
      node = first->monomials->head;
    }
  }
  save_polynomial(id, first);
  is_result_new[id] = 1;
  return SUCCESS_FUNCTION_RETURN;
}

int get_value_by_variable(char* variable, char*** names, double* values, int* size, double* res) {
  for (int i = 0; i < *size; ++i) {
    if (!strcmp(variable, (*names)[i])) {
      *res = values[i];
      return SUCCESS_FUNCTION_RETURN;
    }
  }
  char** possible_name = (char**)realloc(*names, sizeof(char*) * (*size + 1));
  if (possible_name == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  *names = possible_name;
  (*names)[*size] = variable;
  *res = values[*size];
  *size += 1;
  return SUCCESS_FUNCTION_RETURN;
}

int eval(void* data) {
  polynomial* first = ((struct thrd_dt*)data)->poly1;
  int count = ((struct thrd_dt*)data)->second.dots_data.count;
  double* coordinates = ((struct thrd_dt*)data)->second.dots_data.coordinates;
  int id = ((struct thrd_dt*)data)->id;
  if (first == NULL) {
    first = get_memoried_polynomial(id);
    if (first == NULL) {
      return ARGUMENTS_EXCEPTION;
    }
  } else {
    drop_memoried_polynomial(id);
  }
  char** names = NULL;
  int size = 0;
  list_node* node = first->monomials->head;
  double total = first->free_term;
  for (int i = 0; i < first->monomials->size; ++i) {
    double res = 1;
    for (int j = 0; j < node->value->data_size; ++j) {
      double step;
      pair* pr = node->value->unknowns[j];
      if (get_value_by_variable(pr->name, &names, coordinates, &size, &step) == MEMORY_ALLOCATE_EXCEPTION) {
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      double m = step;
      for (int k = 1; k < pr->power; ++k) {
        step *= m;
      }
      res *= step;
    }
    res *= node->value->coefficient;
    total += res;
    node = node->next;
  }
  free_polynomial(first);
  first = create_polynomial();
  if (first == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  first->free_term = total;
  save_polynomial(id, first);
  is_result_new[id] = 1;
  return SUCCESS_FUNCTION_RETURN;
}

int grad(void* data) {
  polynomial* element = ((struct thrd_dt*)data)->poly1;
  int id = ((struct thrd_dt*)data)->id;
  if (element == NULL) {
    element = get_memoried_polynomial(id);
    if (element == NULL) {
      printf("%d\n", 0);
      return SUCCESS_FUNCTION_RETURN;
    }
  }
  char** vars = (char**)malloc(sizeof(char*) * 10);
  if (vars == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  int sz = 10;
  int real_sz = 0;
  list_node* node = element->monomials->head;
  for (int i = 0; i < element->monomials->size; ++i) {
    for (int j = 0; j < node->value->data_size; ++j) {
      int flag = 0;
      pair* pr = node->value->unknowns[j];
      for (int k = 0; k < real_sz; ++k) {
        if (!strcmp(vars[k], pr->name)) {
          flag = 1;
          break;
        }
      }
      if (!flag) {
        if (real_sz == sz) {
          char** possible_vars = (char**)realloc(vars, sizeof(char*) * 2 * sz);
          if (possible_vars == NULL) {
            for (int w = 0; w < real_sz; ++w) {
              free(vars[w]);
            }
            free(vars);
            return MEMORY_ALLOCATE_EXCEPTION;
          }
          sz *= 2;
          vars = possible_vars;
        }
        vars[real_sz] = (char*)malloc(sizeof(char) * strlen(pr->name));
        if (vars[real_sz] == NULL) {
          for (int w = 0; w < real_sz; ++w) {
            free(vars[w]);
          }
          free(vars);
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        strcpy(vars[real_sz], pr->name);
        real_sz += 1;
      }
    }
    node = node->next;
  }
  polynomial** elements = (polynomial**)malloc(sizeof(polynomial*) * real_sz);
  for (int i = 0; i < real_sz; ++i) {
    elements[i] = create_polynomial();
    if (elements[i] == NULL) {
      for (int w = 0; w < real_sz; ++w) {
        free(vars[w]);
      }
      free(vars);
      for (int j = 0; j < i; ++j) {
        free_polynomial(elements[j]);
      }
      free(elements);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if (copy_polynomial(elements[i], element) == MEMORY_ALLOCATE_EXCEPTION) {
      for (int w = 0; w < real_sz; ++w) {
        free(vars[w]);
      }
      free(vars);
      for (int j = 0; j < i; ++j) {
        free_polynomial(elements[j]);
      }
      free(elements);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
  }
  for (int i = 0; i < real_sz; ++i) {
    struct thrd_dt data = { .poly1 = elements[i], .second.var = vars[i], .id = id };
    diff(&data);
    data.poly1 = NULL;
    print(&data);
  }
  save_polynomial(id, elements[real_sz - 1]);
}

int print(void* data) {
  polynomial* element = ((struct thrd_dt*)data)->poly1;
  int id = ((struct thrd_dt*)data)->id;
  if (element == NULL) {
    element = get_memoried_polynomial(id);
    if (element == NULL) {
      printf("%d\n", 0);
      return SUCCESS_FUNCTION_RETURN;
    }
  }
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
  is_result_new[id] = 1;
  return SUCCESS_FUNCTION_RETURN;
}

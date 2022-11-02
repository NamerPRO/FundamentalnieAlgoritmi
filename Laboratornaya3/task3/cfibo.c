#include "cfibo.h"

int create_fibo(fibo* fb) {
  fb->minimum = NULL;
  fb->root_list = (list*)malloc(sizeof(list));
  if (fb->root_list == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  fb->size = 0;
  create_list(fb->root_list);
}

unsigned long int fibo_size(fibo* fb) {
  return fb->size;
}

int fibo_empty(fibo* fb) {
  return !fibo_size(fb);
}

void standart_fibo_clear(list_node* elem) {
  free(elem->value);
  free(elem);
}

int fibo_remove_minimum(fibo* fb, int (*cmp)(FIBO_T lhs, FIBO_T rhs)) {
  int execute_status;
  fibo_node** fb_nodes = fb->minimum->value->brothers;
  unsigned long int brothers_count = fb->minimum->value->brothers_count;
  if ((execute_status = list_remove(fb->root_list, "%i", standart_fibo_clear, fb->minimum)) != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  --fb->size;
  if (fibo_empty(fb)) {
    return SUCCESS_FUNCTION_RETURN;
  }
  for (unsigned long int i = 0; i < brothers_count; ++i) {
    list_push_front(fb->root_list, fb_nodes[i]);
  }
  free(fb_nodes);
  fibo_node** fibo_arr = (fibo_node**)malloc(sizeof(fibo_node*) * 33);
  if (fibo_arr == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (int i = 0; i < 33; ++i) {
    fibo_arr[i] = NULL;
  }
  fibo_node* min_element = NULL;
  while (!list_empty(fb->root_list)) {
    fibo_node* current_element = fb->root_list->head->value;
    list_remove_front(fb->root_list, standart_list_clear);
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    if (min_element == NULL) {
      min_element = current_element;
    } else if (cmp(current_element->value, min_element->value)) {
      min_element = current_element;
    }
    unsigned long int index = current_element->brothers_count;
    if (fibo_arr[index] == NULL) {
      fibo_arr[index] = current_element;
      continue;
    }
    while (fibo_arr[index] != NULL) {
      if (cmp(current_element->value, fibo_arr[index]->value)) {
        if (current_element->brothers_count == current_element->real_brothers_size) {
          fibo_node** possible_brothers = (fibo_node**)realloc(current_element->brothers, sizeof(fibo_node*) * current_element->real_brothers_size * 2);
          if (possible_brothers == NULL) {
            free(fibo_arr);
            return MEMORY_ALLOCATE_EXCEPTION;
          }
          current_element->real_brothers_size *= 2;
          current_element->brothers = possible_brothers;
        }
        current_element->brothers[current_element->brothers_count++] = fibo_arr[index];
      } else {
        if (fibo_arr[index]->brothers_count == fibo_arr[index]->real_brothers_size) {
          fibo_node** possible_brothers = (fibo_node**)realloc(fibo_arr[index]->brothers, sizeof(fibo_node*) * fibo_arr[index]->real_brothers_size * 2);
          if (possible_brothers == NULL) {
            free(fibo_arr);
            return MEMORY_ALLOCATE_EXCEPTION;
          }
          fibo_arr[index]->real_brothers_size *= 2;
          fibo_arr[index]->brothers = possible_brothers;
        }
        fibo_arr[index]->brothers[fibo_arr[index]->brothers_count++] = current_element;
        current_element = fibo_arr[index];
      }
      fibo_arr[index] = NULL;
      ++index;
    }
    fibo_arr[index] = current_element;
  }
  for (int i = 0; i < 33; ++i) {
    if (fibo_arr[i] == NULL) {
      continue;
    }
    list_push_front(fb->root_list, fibo_arr[i]);
    if (fibo_arr[i] == min_element) {
      fb->minimum = fb->root_list->head;
    }
  }
  free(fibo_arr);
  return SUCCESS_FUNCTION_RETURN;
}

FIBO_T fibo_get_minimum(fibo* fb) {
  return fb->minimum->value->value;
}

int standart_fibo_node_comporator(FIBO_T lhs, FIBO_T rhs) {
  return lhs < rhs;
}

int maximum_fibo_node_comporator(FIBO_T lhs, FIBO_T rhs) {
  return rhs < lhs;
}

int fibo_insert(fibo* fb, FIBO_T elem, int (*cmp)(FIBO_T lhs, FIBO_T rhs)) {
  fibo_node* fb_node = (fibo_node*)malloc(sizeof(fibo_node));
  if (fb_node == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  fb_node->value = elem;
  fb_node->brothers_count = 0;
  fb_node->brothers = (fibo_node**)malloc(sizeof(fibo_node*) * STANDART_FIBO_BROTHERS_SIZE);
  if (fb_node->brothers == NULL) {
    free(fb_node);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  fb_node->real_brothers_size = STANDART_FIBO_BROTHERS_SIZE;
  if (fibo_empty(fb)) {
    list_push_front(fb->root_list, fb_node);
    fb->minimum = fb->root_list->head;
    ++fb->size;
    return SUCCESS_FUNCTION_RETURN;
  }
  list_insert(fb->root_list, "%i", fb_node, fb->minimum);
  if (cmp(elem, fb->minimum->value->value)) {
    fb->minimum = fb->minimum->next;
  }
  ++fb->size;
}

void fibo_unite(list* lhs, list* rhs) {
  list_node* node_save = rhs->head->past;
  lhs->head->past->next = rhs->head;
  rhs->head->past = lhs->head->past;
  node_save->next = lhs->head;
  lhs->head->past = node_save;
  lhs->size = rhs->size = lhs->size + rhs->size;
}

fibo* fibo_melt(fibo* fb1, fibo* fb2, int (*cmp)(FIBO_T lhs, FIBO_T rhs)) {
  if (fibo_empty(fb1)) {
    return fb2;
  }
  if (fibo_empty(fb2)) {
    return fb1;
  }
  fibo_unite(fb1->root_list, fb2->root_list);
  fb1->size = fb2->size = fb1->size + fb2->size;
  if (cmp(fb1->minimum->value->value, fb2->minimum->value->value)) {
    free(fb2->root_list);
    return fb1;
  }
  free(fb1->root_list);
  return fb2;
}

void fibo_free(fibo* fb, int (*cmp)(FIBO_T lhs, FIBO_T rhs)) {
  while (!fibo_empty(fb)) {
    fibo_remove_minimum(fb, cmp);
  }
  free(fb->root_list);
}

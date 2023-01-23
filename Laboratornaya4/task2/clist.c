#include "clist.h"

void create_list(list* lst) {
  lst->head = NULL;
  lst->size = 0;
}

unsigned long int list_size(list* lst) {
  return lst->size;
}

int list_empty(list* lst) {
  return !lst->size;
}

int list_insert(list* lst, char* pattern, LIST_T elem, ...) {
  if (list_empty(lst)) {
    lst->head = (list_node*)malloc(sizeof(list_node));
    if (lst->head == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    lst->head->value = elem;
    lst->head->next = lst->head;
    lst->head->past = lst->head;
    lst->size = 1;
    return SUCCESS_FUNCTION_RETURN;
  }
  list_node* lst_node = (list_node*)malloc(sizeof(list_node));
  if (lst_node == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  lst_node->value = elem;
  va_list arg;
  va_start(arg, elem);
  list_node* lst_ptr = lst->head;
  if (pattern[1] == 'd') {
    unsigned long int index = va_arg(arg, unsigned long int);
    if (index > lst->size) {
      return INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (!index) {
      lst_ptr = lst_ptr->past;
      lst->head = lst_node;
    } else if (index == lst->size) {
      lst_ptr = lst->head->past;
    } else {
      for (unsigned long int i = 0; i < index - 1; ++i) {
        lst_ptr = lst_ptr->next;
      }
    }
  } else {
    //вставляет вправо, в то время,
    //как вставляя по индексу мы вставим влево
    lst_ptr = va_arg(arg, list_node*);
    // lst_ptr = lst_ptr->past;
  }
  va_end(arg);
  list_node* lst_save = lst_ptr->next;
  lst_ptr->next = lst_node;
  lst_node->past = lst_ptr;
  lst_node->next = lst_save;
  lst_save->past = lst_node;
  ++lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_push_front(list* lst, LIST_T elem) {
  return list_insert(lst, "%d", elem, 0);
}

int list_push_back(list* lst, LIST_T elem) {
  return list_insert(lst, "%d", elem, lst->size);
}

void standart_list_clear(list_node* elem) {
  free(elem);
}

int list_remove(list* lst, char* pattern, void (*clear)(list_node* elem), ...) {
  va_list arg;
  va_start(arg, clear);
  if (list_empty(lst)) {
    return INDEX_OUT_OF_BOUNDS_EXCEPTION;
  }
  list_node* lst_ptr = lst->head;
  if (pattern[1] == 'd') {
    unsigned long int index = va_arg(arg, unsigned long int);
    if (index >= lst->size) {
      return INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (!index) {
      lst_ptr = lst_ptr->past;
      lst->head = lst->head->next;
    } else if (index == lst->size) {
      lst_ptr = lst->head->past->past;
    } else {
      for (unsigned long int i = 0; i < index - 1; ++i) {
        lst_ptr = lst_ptr->next;
      }
    }
  } else {
    lst_ptr = va_arg(arg, list_node*);
    if (lst_ptr == lst->head) {
      lst->head = lst->head->next;
    }
    lst_ptr = lst_ptr->past;
  }
  va_end(arg);
  list_node* lst_save = lst_ptr->next;
  lst_ptr->next->next->past = lst_ptr;
  lst_ptr->next = lst_ptr->next->next;
  clear(lst_save);
  --lst->size;
  return SUCCESS_FUNCTION_RETURN;
}

int list_remove_front(list* lst, void (*clear)(list_node* elem)) {
  return list_remove(lst, "%d", clear, 0);
}

int list_remove_back(list* lst, void (*clear)(list_node* elem)) {
  return list_remove(lst, "%d", clear, lst->size - 1);
}

int list_free(list* lst, void (*clear)(list_node* elem)) {
  int execute_status;
  while (!list_empty(lst)) {
    if ((execute_status = list_remove_front(lst, clear)) != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

LIST_T list_extract_element(list* lst, char* pattern, int* execute_status, ...) {
  if (list_empty(lst)) {
    *execute_status = INDEX_OUT_OF_BOUNDS_EXCEPTION;
    return 0;
  }
  va_list arg;
  va_start(arg, execute_status);
  list_node* lst_ptr = lst->head;
  if (pattern[1] == 'd') {
    unsigned long int index = va_arg(arg, unsigned long int);
    if (index >= lst->size) {
      *execute_status = INDEX_OUT_OF_BOUNDS_EXCEPTION;
      return 0;
    }
    for (unsigned long int i = 0; i < index; ++i) {
      lst_ptr = lst_ptr->next;
    }
  } else {
    lst_ptr = va_arg(arg, list_node*);
  }
  va_end(arg);
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return lst_ptr->value;
}

list_node* list_extract_pointer(list* lst, char* pattern, int* execute_status, ...) {
  if (list_empty(lst)) {
    *execute_status = INDEX_OUT_OF_BOUNDS_EXCEPTION;
    return 0;
  }
  va_list arg;
  va_start(arg, execute_status);
  list_node* lst_ptr = lst->head;
  if (pattern[1] == 'd') {
    unsigned long int index = va_arg(arg, unsigned long int);
    if (index >= lst->size) {
      *execute_status = INDEX_OUT_OF_BOUNDS_EXCEPTION;
      return 0;
    }
    for (unsigned long int i = 0; i < index; ++i) {
      lst_ptr = lst_ptr->next;
    }
  } else {
    lst_ptr = va_arg(arg, list_node*);
  }
  va_end(arg);
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return lst_ptr;
}

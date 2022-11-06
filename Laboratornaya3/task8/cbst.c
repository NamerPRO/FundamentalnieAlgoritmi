#include "cbst.h"

int create_bst(bst_node** b) {
  *b = (bst_node*)malloc(sizeof(bst_node));
  if (*b == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  (*b)->left = NULL;
  (*b)->right = NULL;
  (*b)->data = (BST_TYPE*)malloc(sizeof(BST_TYPE));
  if ((*b)->data == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  (*b)->data->words = (list*)malloc(sizeof(list));
  if ((*b)->data->words == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  create_list((*b)->data->words);
  (*b)->data->count = 1;
  return SUCCESS_FUNCTION_RETURN;
}

int standart_bst_comporator(BST_KEY lhs, BST_KEY rhs) {
  return (lhs == rhs) ? 0 : (lhs < rhs) ? -1 : 1;
}

bst_node* bst_find(bst_node* b, BST_KEY key, int (*cmp)(BST_KEY lhs, BST_KEY rhs)) {
  while (b != NULL && cmp(b->data->count, key)) {
    if (cmp(key, b->data->count) == -1) {
      b = b->left;
    } else {
      b = b->right;
    }
  }
  return (b == NULL) ? NULL : b;
}

bst_node* bst_insert(bst_node* b, BST_KEY key, int* execute_status, int (*cmp)(BST_KEY lhs, BST_KEY rhs), bst_node** BST_ROOT) {
  int cmp_result;
  while (b != NULL) {
    cmp_result = cmp(key, b->data->count);
    if (cmp_result == -1) {
      if (b->left != NULL) {
        b = b->left;
      } else {
        b->left = (bst_node*)malloc(sizeof(bst_node));
        if (b->left == NULL) {
          *execute_status = MEMORY_ALLOCATE_EXCEPTION;
          return NULL;
        }
        b->left->data = (BST_TYPE*)malloc(sizeof(BST_TYPE));
        if (b->left->data == NULL) {
          *execute_status = MEMORY_ALLOCATE_EXCEPTION;
          return NULL;
        }
        b->left->data->count = key;
        b->left->data->words = (list*)malloc(sizeof(list));
        if (b->left->data->words == NULL) {
          *execute_status = MEMORY_ALLOCATE_EXCEPTION;
          return NULL;
        }
        create_list(b->left->data->words);
        b->left->left = NULL;
        b->left->right = NULL;
        *execute_status = SUCCESS_FUNCTION_RETURN;
        return b->left;
      }
    } else if (cmp_result == 1) {
      if (b->right != NULL) {
        b = b->right;
      } else {
        b->right = (bst_node*)malloc(sizeof(bst_node));
        if (b->right == NULL) {
          *execute_status = MEMORY_ALLOCATE_EXCEPTION;
          return NULL;
        }
        b->right->data = (BST_TYPE*)malloc(sizeof(BST_TYPE));
        if (b->right->data == NULL) {
          *execute_status = MEMORY_ALLOCATE_EXCEPTION;
          return NULL;
        }
        b->right->data->count = key;
        b->right->data->words = (list*)malloc(sizeof(list));
        if (b->right->data->words == NULL) {
          *execute_status = MEMORY_ALLOCATE_EXCEPTION;
          return NULL;
        }
        create_list(b->right->data->words);
        b->right->left = NULL;
        b->right->right = NULL;
        *execute_status = SUCCESS_FUNCTION_RETURN;
        return b->right;
      }
    } else {
      *execute_status = SUCCESS_FUNCTION_RETURN;
      return b;
    }
  }
  create_bst(&b);
  *execute_status = SUCCESS_FUNCTION_RETURN;
  *BST_ROOT = b;
  return b;
}

void standart_bst_clear(BST_TYPE* structure) {
  free(structure->words);
}

int bst_remove(bst_node* b, BST_KEY key, void (*cls)(BST_TYPE* structure), int (*cmp)(BST_KEY lhs, BST_KEY rhs), bst_node** BST_ROOT) {
  int cmp_result;
  bst_node* parent_node = NULL;
  while (b != NULL) {
    cmp_result = cmp(key, b->data->count);
    if (cmp_result == -1) {
      if (b->left != NULL) {
        parent_node = b;
        b = b->left;
      } else {
        return NO_SUCH_ELEMENT_EXCEPTION;
      }
    } else if (cmp_result == 1) {
      if (b->right != NULL) {
        parent_node = b;
        b = b->right;
      } else {
        return NO_SUCH_ELEMENT_EXCEPTION;
      }
    } else {
      break;
    }
  }
  if (b->left == NULL && b->right == NULL) {
    if (parent_node != NULL) {
      if (parent_node->left == b) {
        parent_node->left = NULL;
      } else {
        parent_node->right = NULL;
      }
    } else {
      *BST_ROOT = NULL;
    }
    cls(b->data);
    free(b->data);
    free(b);
    return SUCCESS_FUNCTION_RETURN;
  }
  if (b->left == NULL || b->right == NULL) {
    if (b->left == NULL) {
      if (parent_node != NULL) {
        if (parent_node->left == b) {
          parent_node->left = b->right;
        } else {
          parent_node->right = b->right;
        }
      } else {
        *BST_ROOT = b->right;
      }
    } else {
      if (parent_node != NULL) {
        if (parent_node->left == b) {
          parent_node->left = b->left;
        } else {
          parent_node->right = b->left;
        }
      } else {
        *BST_ROOT = b->left;
      }
    }
    cls(b->data);
    free(b->data);
    free(b);
    return SUCCESS_FUNCTION_RETURN;
  }
  bst_node* replace_node = b->right;
  bst_node* replace_node_parent = b;
  while (replace_node->left != NULL) {
    replace_node_parent = replace_node;
    replace_node = replace_node->left;
  }
  if (replace_node_parent->left == replace_node) {
    replace_node_parent->left = replace_node->right;
  } else {
    replace_node_parent->right = replace_node->right;
  }
  if (parent_node != NULL) {
    if (parent_node->left == b) {
      parent_node->left = replace_node;
    } else {
      parent_node->right = replace_node;
    }
  } else {
    *BST_ROOT = replace_node;
  }
  replace_node->left = b->left;
  replace_node->right = b->right;
  cls(b->data);
  free(b->data);
  free(b);
  return SUCCESS_FUNCTION_RETURN;
}

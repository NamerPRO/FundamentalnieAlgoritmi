#ifndef ___CEXPRESSION___
#define ___CEXPRESSION___

#include <math.h>

#include "cstring.h"
#include "stack.h"
#include "cconstants.h"

int validate_brackets(string* expression);
int is_operator(char arg);
int operand_cmp(int symbol, int index);
tree_node* get_expression_tree(string* expression, int* execute_status, int* linker);
int get_priority(string* symb);
tree_node* create_node(sized_string* value);
void print_tree(tree_node* node, int h);
void free_tree(tree_node* node);
void standart_tree_clear(void* structure);
void print_table_by_expression_tree(tree_node* tr, int n, int* linker);
char evaluate_by_expression_tree(tree_node* node, char* permutation, int n, int* linker);
int validate_expression(string* expression);
int is_inverse(int symbol);
int validate_brackets(string* expression);

#endif

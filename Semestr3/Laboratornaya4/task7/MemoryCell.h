#ifndef ___MEMORY_CELL___
#define ___MEMORY_CELL___

#include "cstring.h"

typedef struct MemoryCell {
  string* name;
  int value;
} MemoryCell;

MemoryCell** get_variables();
void clear_variables();
int get_variables_size();
int get_variable_value(MemoryCell* variable);
string* get_variable_name(MemoryCell* variable);
void set_variable_value(MemoryCell* variable, int value);
MemoryCell* get_variable_or_null(string* name);
int var_name_cmp(const MemoryCell** lhs, const MemoryCell** rhs);
int add_variable(string* name, int value);

#endif

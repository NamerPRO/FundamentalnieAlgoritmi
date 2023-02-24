#ifndef ___VILLAGER___
#define ___VILLAGER___

#include "clist_struct.h"
#include "clist.h"

int standart_villager_insert_comporator(LIST_TYPE lhs, LIST_TYPE rhs);
int read_villager_arr_from_file(FILE* file, list* villager_list, int exit_type);
void standart_villager_clear(LIST_TYPE elem);

#endif

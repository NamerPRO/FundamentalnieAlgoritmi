#include <stdio.h>
#include <ctype.h>

#include "cstring.h"

int toenter(int x) {
  if ((char)x == '\n') {
    return 1;
  }
  return 0;
}

int main() {
  string dinamic_str;
  create_string(&dinamic_str, str("nation"));
  print_string(stdout, &dinamic_str);

  remove_data_from_string(&dinamic_str, 1, 3);
  print_string(stdout, &dinamic_str);

  free_string(&dinamic_str);

  return 0;
}

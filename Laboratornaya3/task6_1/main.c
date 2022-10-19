#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "cconstants.h"
#include "cstring.h"

int main(int argc, char* argv[]) {
  string a, b;
  create_all_empty_strings(2, &a, &b);

  get_random_string(&a, 10, standart_random_character);
  get_random_string(&b, 5, standart_random_character);

  print_string(stdout, "str1 = %s\nstr2 = %s\n", &a, &b);
  free_all_strings(2, &a, &b);

  return 0;
}

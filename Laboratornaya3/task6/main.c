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
  //Статическая строка (не надо чистить)
  string static_str;
  create_string(&static_str, "nation");
  print_string(stdout, &static_str);
  // printf("static_str size = %lu\n", get_string_size(&static_str));
  //---

  //Динамическая строка
  string dinamic_str;
  create_empty_string(&dinamic_str);
  read_string(stdin, &dinamic_str, toenter);
  print_string(stdout, &dinamic_str);
  // printf("dinamic_str size = %lu\n", get_string_size(&dinamic_str));
  // free_string(&dinamic_str);
  // //---
  //
  // int res = string_compare(&static_str, &dinamic_str, standart_string_comporator);
  // printf("Res = %d\n", res);
  //
  // string dup_string;
  // create_string_duplicate(&dinamic_str, &dup_string);
  // free_string(&dinamic_str);
  // printf("Copied string is: %s\n", get_raw_string(&dup_string));
  //
  // free_string(&dup_string);
  // create_string_duplicate(&static_str, &dup_string);
  // printf("Copied string is: %s\n", get_raw_string(&dup_string));
  // free_string(&dup_string);

  // string str1, str2;
  // // create_string(&str1, " ");
  // create_string(&str1, "!");
  //
  // string_concat(3, &dinamic_str, &str1, &static_str);
  // print_string(stdout, &dinamic_str);
  // free_string(&dinamic_str);

  string_copy(&static_str, &dinamic_str);
  print_string(stdout, &dinamic_str);
  free_string(&dinamic_str);


  return 0;
}

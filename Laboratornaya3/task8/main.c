#include <stdio.h>
#include <ctype.h>

#include "ctrie.h"
#include "cbst.h"
#include "cstring.h"
#include "cio.h"
#include "statistics.h"

int file_separator(int symbol) {
  return (symbol == '!' || symbol == '?' || symbol == ' ' || symbol == '.'
          || symbol == ',' || symbol == '\n' || symbol == '\t' || symbol == '\v'
          || symbol == '\f' || symbol == '\r') ? 1 : 0;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  int execute_status;
  trie_node* prefix_tree = NULL;
  if ((execute_status = create_trie(&prefix_tree)) != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  bst_node* BST_ROOT = NULL;
  // string cstr;
  // create_string(&cstr, "mom");
  // trie_insert(prefix_tree, 0, &cstr, &BST_ROOT);
  unsigned long int count;
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    return FILE_INTERRACT_EXCEPTION;
  }
  string* cstr = (string*)malloc(sizeof(string));
  if (cstr == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  create_empty_string(cstr);
  while ((execute_status = read_string(file, cstr, file_separator)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    if (!string_compare("%p", standart_string_comporator, cstr, "")) {
      continue;
    }
    trie_insert(prefix_tree, 0, cstr, &BST_ROOT);
    cstr = (string*)malloc(sizeof(string));
    create_empty_string(cstr);
  }
  string action;
  if ((execute_status = create_empty_string(&action)) != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  print_string(stdout, "================\ncount -> count amount of word occurrences in file\nfind -> find n most common words in file\nlongest -> get longest word from file\nshortest -> get shortest word from file\n================\n");
  while ((execute_status = read_string(stdin, &action, isspace)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      return execute_status;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "count")) {
      string request_str;
      print_string(stdout, "Enter a word: ");
      if ((execute_status = safe_read(stdin, &count, "%s", isspace, &request_str)) != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
      int cnt = count_occurrences(prefix_tree, 0, &request_str);
      if (cnt == -1) {
        print_string(stdout, "There is no word \"%s\" in file!\n", &request_str);
      } else {
        print_string(stdout, "Word \"%s\" occures %d times in text.\n", &request_str, cnt);
      }
      free_string(&request_str);
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "find")) {
      int n, save_n;
      print_string(stdout, "Enter n: ");
      if ((execute_status = safe_read(stdin, &count, "%d", isspace, &n)) != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
      save_n = n;
      list lst;
      create_list(&lst);
      get_most_common_k_words(BST_ROOT, &lst, &n, &execute_status);
      if (execute_status != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
      print_string(stdout, "First %d most common words: ", save_n);
      while (!list_empty(&lst)) {
        string* print_str = list_element_at(&lst, 0, &execute_status);
        if (execute_status != SUCCESS_FUNCTION_RETURN) {
          return execute_status;
        }
        print_string(stdout, "\"%s\"; ", print_str);
        list_pop_front(&lst, standart_clear_function);
      }
      print_string(stdout, "\n");
      if (n != 0) {
        print_string(stdout, "Reached word limit! There are less than %d unique words in text.\n", save_n);
      }
      continue;
    }
    print_string(stdout, "Unknown command! Use one in the box above.\n");
  }
  fclose(file);
  return 0;
}

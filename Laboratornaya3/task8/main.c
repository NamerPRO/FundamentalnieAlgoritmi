#include <stdio.h>
#include <ctype.h>

#include "clist.h"
#include "ctrie.h"
#include "cbst.h"
#include "cstring.h"
#include "cio.h"
#include "statistics.h"

int file_separator(int symbol) {
  return (symbol == '!' || symbol == '?' || symbol == ' ' || symbol == '.'
          || symbol == ',' || symbol == '\n' || symbol == '\t' || symbol == '\v'
          || symbol == '\f' || symbol == '\r' || symbol == ';') ? 1 : 0;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    print_string(stdout, "Incorrect usage! Pass path to file as an only argument.\n");
    return ARGUMENTS_EXCEPTION;
  }
  int execute_status;
  trie_node* prefix_tree = NULL;
  if ((execute_status = create_trie(&prefix_tree)) != SUCCESS_FUNCTION_RETURN) {
    print_string(stdout, "Internal error occured while creating prefix tree! May be not enought memory on your device?\n");
    return execute_status;
  }
  bst_node* BST_ROOT = NULL;
  unsigned long int count;
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    print_string(stdout, "Given wrong path to file! Or there is a problem wile interractiong with it.\n");
    free(prefix_tree);
    return FILE_INTERRACT_EXCEPTION;
  }
  string* cstr = (string*)malloc(sizeof(string));
  if (cstr == NULL) {
    print_string(stdout, "Internal error occured while creating string! May be not enought memory on your device?\n");
    free(prefix_tree);
    fclose(file);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  if ((execute_status = create_empty_string(cstr)) != SUCCESS_FUNCTION_RETURN) {
    print_string(stdout, "Internal error occured while creating string! May be not enought memory on your device?\n");
    free(prefix_tree);
    free(cstr);
    fclose(file);
    return execute_status;
  }
  string longest_string, shortest_string;
  if ((execute_status = create_empty_string(&longest_string)) != SUCCESS_FUNCTION_RETURN) {
    print_string(stdout, "Internal error occured while creating string! May be not enought memory on your device?\n");
    free(prefix_tree);
    free_string(cstr);
    free(cstr);
    fclose(file);
    return execute_status;
  }
  if ((execute_status = create_empty_string(&shortest_string)) != SUCCESS_FUNCTION_RETURN) {
    print_string(stdout, "Internal error occured while creating string! May be not enought memory on your device?\n");
    free(prefix_tree);
    free_string(cstr);
    free(cstr);
    free_string(&longest_string);
    fclose(file);
    return execute_status;
  }
  int shortest_init_flag = 1;
  while ((execute_status = read_string(file, cstr, file_separator)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      print_string(stdout, "Something went wrong while reading a word! Skipping it.\n");
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, cstr, "")) {
      continue;
    }
    unsigned long int cstr_size = get_string_size(cstr);
    unsigned long int longest_string_size = get_string_size(&longest_string);
    unsigned long int shortest_string_size = get_string_size(&shortest_string);
    to_lower_case(cstr);
    if (longest_string_size < cstr_size) {
      string_copy(cstr, &longest_string);
    } else if (longest_string_size == cstr_size) {
      if (string_compare("%s", standart_string_comporator, &longest_string, cstr) == 1) {
        string_copy(cstr, &longest_string);
      }
    }
    if (shortest_init_flag || shortest_string_size > cstr_size) {
      string_copy(cstr, &shortest_string);
      shortest_init_flag = 0;
    } else if (shortest_string_size == cstr_size) {
      if (string_compare("%s", standart_string_comporator, &shortest_string, cstr) == 1) {
        string_copy(cstr, &shortest_string);
      }
    }
    trie_insert(prefix_tree, 0, cstr, &BST_ROOT);
    cstr = (string*)malloc(sizeof(string));
    if (cstr == NULL) {
      print_string(stdout, "Internal error occured while creating string! May be not enought memory on your device?\n");
      free_string(&longest_string);
      free_string(&shortest_string);
      complete_bst_destroy(&BST_ROOT);
      trie_destroy(prefix_tree);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if ((execute_status = create_empty_string(cstr)) != SUCCESS_FUNCTION_RETURN) {
      print_string(stdout, "Internal error occured while creating string! May be not enought memory on your device?\n");
      free(cstr);
      free_string(&longest_string);
      free_string(&shortest_string);
      complete_bst_destroy(&BST_ROOT);
      trie_destroy(prefix_tree);
      return execute_status;
    }
  }
  free_string(cstr);
  free(cstr);
  fclose(file);
  string action;
  if ((execute_status = create_empty_string(&action)) != SUCCESS_FUNCTION_RETURN) {
    print_string(stdout, "Internal error occured while creating string! May be not enought memory on your device?\n");
    complete_bst_destroy(&BST_ROOT);
    trie_destroy(prefix_tree);
    return execute_status;
  }
  print_string(stdout, "================\ncount -> count amount of word occurrences in file\nfind -> find n most common words in file\nlongest -> get longest word from file\nshortest -> get shortest word from file\n================\n");
  while ((execute_status = read_string(stdin, &action, isspace)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      print_string(stdout, "Something went wrong while reading an action! Try again.\n");
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "count")) {
      string request_str;
      print_string(stdout, "Enter a word: ");
      if ((execute_status = safe_read(stdin, &count, "%s", isspace, &request_str)) != SUCCESS_FUNCTION_RETURN) {
        print_string(stdout, "Something went wrong while reading a word! Action rejected.\n");
        free_string(&request_str);
        continue;
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
        print_string(stdout, "Invalid number entered! Action rejected.\n");
        continue;
      }
      save_n = n;
      list lst;
      create_list(&lst);
      get_most_common_k_words(BST_ROOT, &lst, &n, &execute_status);
      if (execute_status != SUCCESS_FUNCTION_RETURN) {
        print_string(stdout, "Something went wrong while getting most common words! Action rejected.\n");
        list_clear(&lst, clear_list_type);
        continue;
      }
      print_string(stdout, "First %d most common words: ", save_n);
      while (!list_empty(&lst)) {
        string* print_str = list_element_at(&lst, 0, &execute_status);
        if (execute_status != SUCCESS_FUNCTION_RETURN) {
          print_string(stdout, "Could not get certain element from list of most common words! Skipping it.\n");
          list_pop_front(&lst, standart_clear_function);
          continue;
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
    if (!string_compare("%p", standart_string_comporator, &action, "longest")) {
      print_string(stdout, "Longest word in file is \"%s\".\n", &longest_string);
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "shortest")) {
      print_string(stdout, "Shortest word in file is \"%s\".\n", &shortest_string);
      continue;
    }
    print_string(stdout, "Unknown command! Use one in the box above.\n");
  }
  free_string(&longest_string);
  free_string(&shortest_string);
  free_string(&action);
  //Пункт 2: определение глубины дерева
  print_string(stdout, "Tree height is %d.\n", get_bst_height(BST_ROOT));
  //Пункт 3: сохранение и восстановление бинарного дерева в/из файл(а)
  file = fopen("save_bst_file", "w");
  if (file == NULL) {
    return FILE_INTERRACT_EXCEPTION;
  }
  save_bst(file, BST_ROOT, &execute_status);
  fclose(file);
  complete_bst_destroy(&BST_ROOT);
  file = fopen("save_bst_file", "r");
  if (file == NULL) {
    return FILE_INTERRACT_EXCEPTION;
  }
  restore_bst(file, &BST_ROOT);
  fclose(file);
  file = fopen("save_bst_file_recover", "w");
  if (file == NULL) {
    return FILE_INTERRACT_EXCEPTION;
  }
  save_bst(file, BST_ROOT, &execute_status);
  fclose(file);
  complete_bst_destroy(&BST_ROOT);
  trie_destroy(prefix_tree);
  return 0;
}

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define WRONG_ARGUMENTS_EXCEPTION 1
#define COUNT_EXCEPTION 2

typedef struct file_pair {
  char* name;
  int start;
} file_pair;

int structloc(file_pair*** structure, int size, int new_size) {
  file_pair** ptr_structure = (file_pair**)realloc(*structure, sizeof(file_pair*) * (size + new_size));
  if (ptr_structure == NULL) {
    return -1;
  }
  *structure = ptr_structure;
  for (int i = size; i < size + new_size; ++i) {
    if (((*structure)[i] = (file_pair*)malloc(sizeof(file_pair))) == NULL) {
      return -1;
    }
  }
  return 0;
}

void count_coincidences(int* total_elements_allocated, file_pair*** result, int* count, char* substring, int amount_of_files, ...) {
  int elems = *total_elements_allocated = 50;
  if (structloc(result, 0, elems)) {
    *count = -1;
    return;
  }
  int amount_of_coincedences = 0;
  int substring_length = strlen(substring);
  va_list file_paths;
  va_start(file_paths, amount_of_files);
  char* path;
  for (int i = 0; i < amount_of_files; ++i) {
    path = va_arg(file_paths, char*);
    FILE* file_ptr1 = NULL;
    FILE* file_ptr2 = NULL;
    if ((file_ptr1 = fopen(path, "r")) == NULL) {
      *count = -1;
      return;
    }
    if ((file_ptr2 = fopen(path, "r")) == NULL) {
      *count = -1;
      return;
    }
    char symbol;
    int position_in_file = 0;
    while (!feof(file_ptr1)) {
      int j = (symbol == substring[0]) ? 1 : 0;
      while (j < substring_length && ((symbol = fgetc(file_ptr1)) == substring[j])) {
        ++j;
        ++position_in_file;
      }
      if (j < substring_length) {
        position_in_file += (symbol == '\n') ? 0 : 1;
      }
      if (j == substring_length) {
        if (amount_of_coincedences % elems == 0) {
          if (structloc(result, amount_of_coincedences, elems)) {
            *count = -1;
            return;
          }
          *total_elements_allocated += elems;
        }
        (*result)[amount_of_coincedences]->name = path;
        (*result)[amount_of_coincedences]->start = position_in_file - substring_length;
        ++amount_of_coincedences;
        fseek(file_ptr2, 1, SEEK_CUR);
        memcpy(file_ptr1, file_ptr2, sizeof(file_ptr2));
      }
    }
    fclose(file_ptr1);
    fclose(file_ptr2);
  }
  va_end(file_paths);
  *count = amount_of_coincedences;
}

int main() {
  int amount_of_coincedences;
  int total_elements_allocated;
  file_pair** result = NULL;
  count_coincidences(&total_elements_allocated, &result, &amount_of_coincedences, "123", 3, "file1", "file2", "file3");
  if (amount_of_coincedences == -1) {
    return COUNT_EXCEPTION;
  }
  for (int i = 0; i < amount_of_coincedences; ++i) {
    printf("Found occurrence in %s at start position %d\n", result[i]->name, result[i]->start);
  }
  printf("There were total %d occurrences found!\n", amount_of_coincedences);
  for (int i = 0; i < total_elements_allocated - 1; ++i) {
    // free(result[i]->name);
    free(result[i]);
  }
  free(result);
  return 0;
}

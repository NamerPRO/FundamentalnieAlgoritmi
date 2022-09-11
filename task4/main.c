#include <stdio.h>

#define ARGUMENTS_EXCEPTION 1
#define FILE_EXCEPTION 2

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }

  FILE* working_file = NULL;
  if ((working_file = fopen(argv[1], "r")) == NULL) {
    return FILE_EXCEPTION;
  }

  FILE* file1 = NULL;
  FILE* file2 = NULL;
  FILE* file3 = NULL;

  if ((file1 = fopen("./f1", "w")) == NULL) {
    return FILE_EXCEPTION;
  }

  if ((file2 = fopen("./f2", "w")) == NULL) {
    return FILE_EXCEPTION;
  }

  if ((file3 = fopen("./f3", "w")) == NULL) {
    return FILE_EXCEPTION;
  }

  char string[255];
  int word_position = 1;
  while (fscanf(working_file, "%s", string) == 1) {
    switch (word_position) {
      case 1:
        fprintf(file1, "%s ", string);
        word_position = 2;
        break;
      case 2:
        fprintf(file2, "%s ", string);
        word_position = 3;
        break;
      case 3:
        fprintf(file3, "%s ", string);
        word_position = 1;
        break;
    }
  }

  fclose(working_file);
  fclose(file1);
  fclose(file2);
  fclose(file3);

  if ((working_file = fopen(argv[1], "w")) == NULL) {
    return FILE_EXCEPTION;
  }

  if ((file1 = fopen("./f1", "r")) == NULL) {
    return FILE_EXCEPTION;
  }

  if ((file2 = fopen("./f2", "r")) == NULL) {
    return FILE_EXCEPTION;
  }

  if ((file3 = fopen("./f3", "r")) == NULL) {
    return FILE_EXCEPTION;
  }

  word_position = 1;
  while (!feof(file2)) {
    switch (word_position) {
      case 1:
        fscanf(file3, "%s", string);
        word_position = 2;
        break;
      case 2:
        fscanf(file1, "%s", string);
        word_position = 3;
        break;
      case 3:
        fscanf(file2, "%s% c", string);
        word_position = 1;
        break;
    }
    fprintf(working_file, "%s%c", string, (word_position == 1) ? '\n' : ' ');
  }

  fclose(file1);
  fclose(file2);
  fclose(file3);

  remove("f1");
  remove("f2");
  remove("f3");

  fclose(working_file);

  return 0;
}

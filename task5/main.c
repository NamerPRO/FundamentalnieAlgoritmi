#include <stdio.h>

#define ARGUMENTS_EXCEPTION 1
#define FILE_EXCEPTION 2
#define UNKNOWN_FLAG_EXCEPTION 3

typedef enum FLAG_TYPE {
  FI,
  CIN,
  ARG,
  UNKNOWN
} FLAG_TYPE;

FLAG_TYPE get_flag_type(char* str) {
  if (!strcmp(str, "-fi")) {
    return FI;
  }
  if (!strcmp(str, "-cin")) {
    return CIN;
  }
  if (!strcmp(str, "-arg")) {
    return ARG;
  }
  return UNKNOWN;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return ARGUMENTS_EXCEPTION;
  }

  FLAG_TYPE flag_type = get_flag_type(argv[1]);

  char file_name[255];
  FILE* output_file = NULL;
  FILE* file_with_data = NULL;

  switch (flag_type) {
    case FI:
      if (argc != 3) {
        return ARGUMENTS_EXCEPTION;
      }
      FILE* input_file = NULL;
      if ((input_file = fopen(argv[2], "r")) == NULL) {
        return FILE_EXCEPTION;
      }
      if ((output_file = fopen("./output_file", "w")) == NULL) {
        return FILE_EXCEPTION;
      }
      while (fscanf(input_file, "%s", file_name) == 1) {
        if ((file_with_data = fopen(file_name, "r")) == NULL) {
          return FILE_EXCEPTION;
        }
        char symbol;
        while ((symbol = fgetc(file_with_data)) != EOF) {
          if (symbol == '\n') {
            continue;
          }
          putc(symbol, output_file);
        }
        fclose(file_with_data);
      }
      fclose(input_file);
      fclose(output_file);
      break;
    case CIN:
      if (argc != 2) {
        return ARGUMENTS_EXCEPTION;
      }
      if ((output_file = fopen("./output_file", "w")) == NULL) {
        return FILE_EXCEPTION;
      }
      while (scanf("%s", file_name) == 1) {
        if ((file_with_data = fopen(file_name, "r")) == NULL) {
          return FILE_EXCEPTION;
        }
        char symbol;
        while ((symbol = fgetc(file_with_data)) != EOF) {
          if (symbol == '\n') {
            continue;
          }
          putc(symbol, output_file);
        }
        fclose(file_with_data);
      }
      fclose(output_file);
      break;
    case ARG:
      if (argc < 3) {
        return ARGUMENTS_EXCEPTION;
      }
      if ((output_file = fopen("./output_file", "w")) == NULL) {
        return FILE_EXCEPTION;
      }
      for (int i = 2; i < argc; ++i) {
        if ((file_with_data = fopen(argv[i], "r")) == NULL) {
          return FILE_EXCEPTION;
        }
        char symbol;
        while ((symbol = fgetc(file_with_data)) != EOF) {
          if (symbol == '\n') {
            continue;
          }
          putc(symbol, output_file);
        }
        fclose(file_with_data);
      }
      fclose(output_file);
      break;
    case UNKNOWN:
      return UNKNOWN_FLAG_EXCEPTION;
      break;
  }

  return 0;
}

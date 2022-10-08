#include <stdio.h>
#include <limits.h> //#import <linux/limits.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

#include "overflow.h"

#define PATH_MAX 4096

#define WRONG_ARGUMENTS_EXCEPTION 1
#define WRONG_NUMBER_EXCEPTION 2
#define VARIABLE_OVERFLOWED_EXCEPTION 3
#define FILE_EXCEPTION 4
#define UNKNOWN_FLAG_EXCEPTION 5

const double EPSILON = 0.00001;

typedef enum FLAG_TYPE {
  D,
  I,
  S,
  A,
  F,
  ND,
  NS,
  NI,
  NA,
  NF,
  UNKNOWN
} FLAG_TYPE;

FLAG_TYPE get_flag_type(char* str) {
  if (!strcmp(str, "-d") || !strcmp(str, "/d")) {
    return D;
  }
  if (!strcmp(str, "-i") || !strcmp(str, "/i")) {
    return I;
  }
  if (!strcmp(str, "-s") || !strcmp(str, "/s")) {
    return S;
  }
  if (!strcmp(str, "-a") || !strcmp(str, "/a")) {
    return A;
  }
  if (!strcmp(str, "-f") || !strcmp(str, "/f")) {
    return F;
  }
  if (!strcmp(str, "-nd") || !strcmp(str, "/nd")) {
    return ND;
  }
  if (!strcmp(str, "-ni") || !strcmp(str, "/ni")) {
    return NI;
  }
  if (!strcmp(str, "-ns") || !strcmp(str, "/ns")) {
    return NS;
  }
  if (!strcmp(str, "-na") || !strcmp(str, "/na")) {
    return NA;
  }
  if (!strcmp(str, "-nf") || !strcmp(str, "/nf")) {
    return NF;
  }
  return UNKNOWN;
}

void to_lower_case(char* string) {
  for (int i = 0; string[i] != '\0'; ++i) {
    if (isalpha(string[i])) {
      string[i] = tolower(string[i]);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    return WRONG_ARGUMENTS_EXCEPTION;
  }
  FLAG_TYPE flag = get_flag_type(argv[1]);
  char path[PATH_MAX];
  strcpy(path, dirname(argv[2]));
  if (flag == ND || flag == NI || flag == NS || flag == NA || flag == NF) {
    if (argc != 4) {
      return WRONG_ARGUMENTS_EXCEPTION;
    }
    strcat(path, "/");
    strcat(path, argv[3]);
  } else {
    if (argc != 3) {
      return WRONG_ARGUMENTS_EXCEPTION;
    }
    strcat(path, "/out_");
    strcat(path, basename(argv[2]));
  }
  FILE* input_file = NULL;
  if ((input_file = fopen(argv[2], "r")) == NULL) {
    return FILE_EXCEPTION;
  }
  FILE* output_file = NULL;
  if ((output_file = fopen(path, "w")) == NULL) {
    fclose(input_file);
    return FILE_EXCEPTION;
  }
  char symbol;
  switch (flag) {
  case ND:
  case D:
    while (!feof(input_file)) {
      fscanf(input_file, "%c", &symbol);
      if (!isdigit(symbol)) {
        fprintf(output_file, "%c", symbol);
      }
    }
    break;
  case NI:
  case I: ;
    unsigned int count_letters = 0;
    while ((symbol = fgetc(input_file)) != EOF) {
      //fscanf(input_file, "%c", &symbol);
      if (symbol == '\n') {
        fprintf(output_file, "Total amount of letters on line: %u\n", count_letters);
        count_letters = 0;
        continue;
      }
      if (isalpha(symbol)) {
        // ++count_letters;
        int is_overflowed_letters = 0;
        count_letters = safe_sum(count_letters, 1, &is_overflowed_letters);
        if (is_overflowed_letters) {
          fclose(input_file);
          fclose(output_file);
          return VARIABLE_OVERFLOWED_EXCEPTION;
        }
      }
    }
    break;
  case NS:
  case S: ;
    unsigned int count_symbols = 0;
    while ((symbol = fgetc(input_file)) != EOF) {
      //fscanf(input_file, "%c", &symbol);
      if (symbol == '\n') {
        fprintf(output_file, "Total amount of required symbols on line: %u\n", count_symbols);
        count_symbols = 0;
        continue;
      }
      if (!isdigit(symbol) && !isalpha(symbol) && symbol != ' ') {
        // ++count_symbols;
        int is_overflowed_symbols = 0;
        count_symbols = safe_sum(count_symbols, 1, &is_overflowed_symbols);
        if (is_overflowed_symbols) {
          fclose(input_file);
          fclose(output_file);
          return VARIABLE_OVERFLOWED_EXCEPTION;
        }
      }
    }
    break;
  case NA:
  case A:
    while ((symbol = fgetc(input_file)) != EOF) {
      //fscanf(input_file, "%c", &symbol);
      if (isdigit(symbol) || symbol == '\n') {
        fprintf(output_file, "%c", symbol);
        continue;
      }
      fprintf(output_file, "%d", symbol);
    }
    break;
  case NF:
  case F: ;
    int string_counter = 1;
    char string[255];
    int str_length;
    while (!feof(input_file)) {
      string[0] = '\0';
      str_length = 0;
      while ((symbol = fgetc(input_file)) != EOF) {
        if (!isspace(symbol)) {
          string[str_length++] = symbol;
          continue;
        }
        break;
      }
      string[str_length] = '\0';
      if (string_counter % 2 == 0 && string_counter % 5 != 0) {
        to_lower_case(string);
        fprintf(output_file, "%s", string);
      } else if (string_counter % 2 != 0 && string_counter % 5 == 0) {
        for (unsigned int i = 0; string[i] != '\0'; ++i) {
          fprintf(output_file, "%u", string[i]);
        }
      } else if (string_counter % 2 == 0 && string_counter % 5 == 0) {
        to_lower_case(string);
        for (unsigned int i = 0; string[i] != '\0'; ++i) {
          fprintf(output_file, "%u", string[i]);
        }
      } else {
        fprintf(output_file, "%s", string);
      }
      if (symbol != EOF) {
        fprintf(output_file, "%c", symbol);
      }
      if (string_counter == 10) {
        string_counter = 1;
      } else {
        ++string_counter;
      }
    }
    break;
  case UNKNOWN:
    fclose(input_file);
    fclose(output_file);
    return UNKNOWN_FLAG_EXCEPTION;
  }
  fclose(input_file);
  fclose(output_file);
  return 0;
}

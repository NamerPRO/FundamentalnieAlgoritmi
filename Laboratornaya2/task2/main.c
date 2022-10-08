#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ARGUMENTS_EXCEPTION 1
#define UNKNOWN_FLAG_EXCEPTION 2

#define MAX_STR_SIZE 255

typedef enum FLAG_TYPE {
  L,
  R,
  U,
  N,
  C,
  UNKNOWN
} FLAG_TYPE;

FLAG_TYPE get_flag_type(char* str) {
  if (!strcmp(str, "-l")) {
    return L;
  }
  if (!strcmp(str, "-r")) {
    return R;
  }
  if (!strcmp(str, "-u")) {
    return U;
  }
  if (!strcmp(str, "-n")) {
    return N;
  }
  if (!strcmp(str, "-c")) {
    return C;
  }
  return UNKNOWN;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    return ARGUMENTS_EXCEPTION;
  }
  FLAG_TYPE flag = get_flag_type(argv[2]);
  if (flag == C) {
    if (argc != 4) {
      return ARGUMENTS_EXCEPTION;
    }
  } else if (argc != 3) {
    return ARGUMENTS_EXCEPTION;
  }
  switch (flag) {
    case L: ;
      unsigned int length = 0;
      for (; argv[1][length] != '\0'; ++length) { }
      printf("Length = %u\n", length);
      break;
    case R: ;
      char string[MAX_STR_SIZE];
      char* string_ptr = string + MAX_STR_SIZE - 1;
      *string_ptr = '\0';
      for (int i = 0; argv[1][i] != '\0'; ++i) {
        *--string_ptr = argv[1][i];
      }
      printf("%s\n", string_ptr);
      break;
    case U:
      for (int i = 1; argv[1][i - 1] != '\0' && argv[1][i] != '\0'; i += 2) {
        argv[1][i] = (char)toupper(argv[1][i]);
      }
      printf("%s\n", argv[1]);
      break;
    case N: ;
      char changed_string[MAX_STR_SIZE];
      int j = 0;
      for (int i = 0; argv[1][i] != '\0'; ++i) {
        if (isdigit(argv[1][i])) {
          changed_string[j++] = argv[1][i];
        }
      }
      for (int i = 0; argv[1][i] != '\0'; ++i) {
        if (isalpha(argv[1][i])) {
          changed_string[j++] = argv[1][i];
        }
      }
      for (int i = 0; argv[1][i] != '\0'; ++i) {
        if (!isdigit(argv[1][i]) && !isalpha(argv[1][i])) {
          changed_string[j++] = argv[1][i];
        }
      }
      changed_string[j] = '\0';
      printf("%s\n", changed_string);
      break;
    case C: ;
      char concat_string[MAX_STR_SIZE];
      char* concat_string_ptr = concat_string;
      for (int i = 0; argv[1][i] != '\0'; ++i) {
        *concat_string_ptr++ = argv[1][i];
      }
      for (int i = 0; argv[3][i] != '\0'; ++i) {
        *concat_string_ptr++ = argv[3][i];
      }
      *concat_string_ptr = '\0';
      printf("%s\n", concat_string);
      break;
    default:
      return UNKNOWN_FLAG_EXCEPTION;
  }
  return 0;
}

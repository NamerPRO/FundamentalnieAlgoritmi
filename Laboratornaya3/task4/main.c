#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cconstants.h"
#include "cmessage.h"
#include "cstring.h"
#include "cio.h"

#define ARGUMENTS_EXCEPTION 7
#define FILE_EXCEPTION 8

int read_to_enter(int x) {
  if ((char)x == '\n') {
    return 1;
  }
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  string file_name;
  if (create_empty_string(&file_name) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  srand(time(NULL));
  if (get_random_string(&file_name, rand() % 6 + 5, standart_random_character) == MEMORY_ALLOCATE_EXCEPTION) {
    free_string(&file_name);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  if (string_concat("%p", &file_name, ".csv") == MEMORY_ALLOCATE_EXCEPTION) {
    free_string(&file_name);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  FILE* file = fopen(get_raw_string(&file_name), "w");
  if (file == NULL) {
    free_string(&file_name);
    return FILE_EXCEPTION;
  }
  string str_msg, exit_word;
  int execute_status;
  create_string(&exit_word, str(argv[1], &execute_status));
  if (execute_status != SUCCESS_FUNCTION_RETURN) {
    fclose(file);
    free_string(&file_name);
    return execute_status;
  }
  printf("Enter your messages:\n");
  while ((execute_status = safe_read(stdin, "%s", read_to_enter, &str_msg)) == SUCCESS_FUNCTION_RETURN) {
    if (!string_compare(&str_msg, &exit_word, standart_string_comporator)) {
      break;
    }
    message msg;
    create_message(&msg, &str_msg);
    fprintf(file, "%ld,%s,%ld\n", get_message_id(&msg), get_raw_string(get_message_text(&msg)), get_message_size(&msg));
    free_message(&msg);
  }
  fclose(file);
  free_string(&str_msg);
  free_string(&exit_word);
  if (execute_status != SUCCESS_FUNCTION_RETURN) {
    free_string(&file_name);
    return execute_status;
  }
  file = fopen(get_raw_string(&file_name), "r");
  free_string(&file_name);
  if (file == NULL) {
    return FILE_EXCEPTION;
  }
  message* msg_arr;
  unsigned long int size;
  if (read_messages_from_file(file, &msg_arr, &size) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  printf("Printed message array:\n");
  print_message_array(msg_arr, size);
  for (unsigned long int i = 0; i < size; ++i) {
    free_string(msg_arr[i].text);
    free(msg_arr[i].text);
  }
  free(msg_arr);
  fclose(file);
  return 0;
}

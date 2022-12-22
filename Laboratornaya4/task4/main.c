#include <stdio.h>

#include "cstring.h"
#include "cconstants.h"
#include "standart_commands_lib.h"
#include "commands_api.h"

int command_end(int symbol, int index) {
  return symbol == '\n' || symbol == EOF;
}


int main(int argc, char* argv[]) {
  if (argc != 2) {
    print_string(stdout, "ERROR! No file with instructions given. Cannot start.\n");
    return ARGUMENTS_EXCEPTION;
  }

  //========
  const int COMMANDS_NUMBER = 11;
  char* commands_list[] = {"load", "save", "rand", "concat", "free", "remove", "copy", "sort", "shuffle", "stats", "print"};
  void (*commands_action[])(string* arguments, unsigned long int start) = {load, save, _rand, concat, _free, _remove, copy, sort, shuffle, stats, print};
  //========

  string* command = heap_string(EMPTY);
  if (command == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    free_heap_string(command);
    return FILE_INTERRACT_EXCEPTION;
  }
  int execute_status;
  unsigned long int pos = 0;
  int flag = 0;
  string* command_name = heap_string(EMPTY);
  if (command_name == NULL) {
    free_heap_string(command);
    fclose(file);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  while ((execute_status = read_string(file, command, command_end)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      free_heap_string(command_name);
      free_heap_string(command);
      fclose(file);
      return execute_status;
    }
    pos = 0;
    if ((execute_status = read_string(FROM_STRING, command, command_next, command_name, &pos)) != SUCCESS_FUNCTION_RETURN) {
      free_heap_string(command_name);
      free_heap_string(command);
      fclose(file);
      return execute_status;
    }
    to_lower_case(command_name);
    flag = 0;
    for (int i = 0; i < COMMANDS_NUMBER; ++i) {
        if (!scompare(command_name, commands_list[i])) {
          commands_action[i](command, pos);
          flag = 1;
          break;
        }
    }
    if (!flag) {
      print_string(stdout, "Unknown command \"%s\"! Try again.\n", command_name);
    }
  }
  fclose(file);
  free_heap_string(command);
  free_heap_string(command_name);
  kill_variables();
  return 0;
}

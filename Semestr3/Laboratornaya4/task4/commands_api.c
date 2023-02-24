#include "commands_api.h"

int command_next(int symbol, int index) {
  return symbol == ' ' || symbol == '\n' || symbol == ',' || symbol == '\0' || symbol == '(' || symbol == ')' || symbol == ';' || symbol == '+' || symbol == '-';
}

int get_next_argument(string* argument, string* arguments, unsigned long int* start) {
  if (read_string(FROM_STRING, arguments, command_next, argument, start) != SUCCESS_FUNCTION_RETURN) {
    return 0;
  }
  return sat(arguments, *start - 1);
}

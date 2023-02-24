#include "cmessage.h"

message* get_message_array(message_array* array) {
  return array->array;
}

unsigned long int get_message_array_size(message_array* array) {
  return array->real_size;
}

void create_message(message* msg, string* str) {
  msg->text = str;
  static unsigned long int id = 1;
  msg->id = id++;
  msg->byte_size = get_string_size(str) * sizeof(char);
}

int coma_separator(int x) {
  return (x == ',' || x == '\n') ? 1 : 0;
}

int create_message_array(message_array* array) {
  array->array = (message*)malloc(sizeof(message) * STANDART_ARRAY_SIZE);
  if (array->array == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  array->total_size = STANDART_ARRAY_SIZE;
  array->real_size = 0;
  return SUCCESS_FUNCTION_RETURN;
}

int insert_message_in_array(message_array* array, message element) {
  if (array->real_size == array->total_size) {
    message* possible_array = (message*)realloc(array->array, 2 * sizeof(message) * array->total_size);
    if (possible_array == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    array->total_size *= 2;
    array->array = possible_array;
  }
  array->array[array->real_size++] = element;
  return SUCCESS_FUNCTION_RETURN;
}

void print_message_array(message* msg_arr, unsigned long int size) {
  for (unsigned long int i = 0; i < size; ++i) {
    printf("%ld, %s, %ld\n", msg_arr[i].id, get_raw_string(msg_arr[i].text), msg_arr[i].byte_size);
  }
}

int read_messages_from_file(FILE* file, message** result_msg_arr, unsigned long int* size) {
  message_array msg_arr;
  if (create_message_array(&msg_arr) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  int id, byte_size;
  int execute_status, state = 0;
  message msg;
  while (1) {
    string* str = (string*)malloc(sizeof(string));
    if (create_empty_string(str) == MEMORY_ALLOCATE_EXCEPTION) {
      for (unsigned long int i = 0; i < get_message_array_size(&msg_arr); ++i) {
        free_string(msg_arr.array[i].text);
        free(msg_arr.array[i].text);
      }
      free_string(str);
      free(str);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    if ((execute_status = read_string(file, str, coma_separator)) != SUCCESS_FUNCTION_RETURN) {
      free_string(str);
      free(str);
      break;
    }
    switch (state) {
      case 0:
        id = get_digit_from_string(str, &execute_status);
        free_string(str);
        free(str);
        msg.id = id;
        state = 1;
        break;
      case 1:
        msg.text = str;
        state = 2;
        break;
      case 2:
        byte_size = get_digit_from_string(str, &execute_status);
        free_string(str);
        free(str);
        msg.byte_size = byte_size;
        if (insert_message_in_array(&msg_arr, msg) == MEMORY_ALLOCATE_EXCEPTION) {
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        state = 0;
        break;
    }
  }
  if (execute_status != EOF) {
    for (unsigned long int i = 0; i < get_message_array_size(&msg_arr); ++i) {
      free_string(msg_arr.array[i].text);
      free(msg_arr.array[i].text);
    }
    return execute_status;
  }
  *result_msg_arr = get_message_array(&msg_arr);
  *size = get_message_array_size(&msg_arr);
  return SUCCESS_FUNCTION_RETURN;
}

unsigned long int get_message_id(message* msg) {
  return msg->id;
}

string* get_message_text(message* msg) {
  return msg->text;
}

void free_message(message* msg) {
  free_string(get_message_text(msg));
}

unsigned long int get_message_size(message* msg) {
  return msg->byte_size;
}

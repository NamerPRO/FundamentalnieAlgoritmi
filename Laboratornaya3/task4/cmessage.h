#ifndef ___CMESSAGE___
#define ___CMESSAGE___

#include "cstring.h"
#include "cinteger.h"
#include "cio.h"
#include "coverflow.h"

#define STANDART_ARRAY_SIZE 10

typedef struct message {
  unsigned long int id;
  string* text;
  unsigned long int byte_size;
} message;

typedef struct message_array {
  message* array;
  unsigned long int total_size;
  unsigned long int real_size;
} message_array;

void create_message(message* msg, string* str);
unsigned long int get_message_id(message* msg);
string* get_message_text(message* msg);
unsigned long int get_message_size(message* msg);
void free_message(message* msg);
void print_message_array(message* msg_arr, unsigned long int size);
int read_messages_from_file(FILE* file, message** result_msg_arr, unsigned long int* size);
int insert_message_in_array(message_array* array, message element);
int create_message_array(message_array* array);
int coma_separator(int x);
unsigned long int get_message_array_size(message_array* array);
message* get_message_array(message_array* array);


#endif

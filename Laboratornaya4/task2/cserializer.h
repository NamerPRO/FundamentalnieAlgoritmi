#ifndef ___CSERIALIZER___
#define ___CSERIALIZER___

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "clist.h"
#include "cconstants.h"
#include "cstring.h"
#include "cnumber.h"
#include "cmonomial.h"
#include "cpolynomial.h"

#define STANDART_SERIALIZED_STRING_SIZE 255
#define STANDART_EVAL_PRECITION 3

typedef struct serialized_string {
  char* line;
  int data_size;
  int struct_size;
} serialized_string;

struct dereq {
  char* instruction;
  int amount;
  polynomial** polynomials;
  struct {
    int count;
    double* coordinates;
  }* dot;
};

int flush_serialized_string(serialized_string* s_string, char* line, ...);
int pour_in_serialized_string(serialized_string* pour_in, serialized_string* from, int indent);
serialized_string* create_serialized_string(string* line, ...);
char* get_raw_serialized_string(serialized_string* s_string);
void free_serialized_string(serialized_string* s_string);
int serialize_char(char symbol, serialized_string* s_string);
int serialize_integer(int number, serialized_string* s_string);
int serialize_double(double number, int precition, serialized_string* s_string);
int serialize_string(char* c_string, serialized_string* s_string);
char deserialize_char(serialized_string* s_string, int* index);
int deserialize_integer(serialized_string* s_string, int* index);
double deserialize_double(serialized_string* s_string, int precition, int* index);
char* deserialize_string(serialized_string* s_string, int* index);
int serialize_monomial(serialized_string* s_string, monomial* element);
monomial* deserialize_monomial(serialized_string* s_string, int* index);
int serialize_polynomial(serialized_string* s_string, polynomial* element);
polynomial* deserialize_polynomial(serialized_string* s_string, int* index);
int get_serialized_string_size(serialized_string* s_string);
int serialize_request(serialized_string* s_string, string* request);
struct dereq* deserialize_request(serialized_string* s_string, int* index);
int create_serialized_string_array(serialized_string*** s_array, int size);
void free_serialized_string_array(serialized_string** s_array, int size);
void free_request(struct dereq* request);

#ifdef ___DEBUG___

void print_deserialized_request(struct dereq* request);

#endif

#endif

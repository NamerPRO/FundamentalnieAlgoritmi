#ifndef ___CPOLYMATH___
#define ___CPOLYMATH___

#include <stdlib.h>
#include <math.h>

#include "hash_table_polynomial.h"
#include "cpolynomial.h"
#include "cconstants.h"
#include "cserializer.h"

#define IDS_AMOUNT 1000

#define ID_FREE 1
#define ID_GET 2

int ids[IDS_AMOUNT];
int sem_ids;

//Theses arrays are sync by index
//Index obtained from id_ctl
serialized_string** requests_storage;
polynomial** response_storage; //Also an adder; Unique for each client
int* is_result_new; //Each element is 1 if result exists and was never requested
//===

struct thrd_dt {
  polynomial* poly1;
  union {
    polynomial* poly2;
    char* var;
    struct {
      int count;
      int* coordinates;
    } dots_data;
  } second;
  int id;
};

polynomial* get_memoried_polynomial(int id);
void save_polynomial(int id, polynomial* poly);
void drop_memoried_polynomial(int id);
int add(void* data);
int sub(void* data);
int mult(void* data);
int mult_monomials(monomial* first, monomial* second);
int get_value_by_variable(char* variable, char*** names, double* values, int* size, double* res);
int diff(void* data);
int eval(void* data);
int print(void* data);
int grad(void* data);

#endif

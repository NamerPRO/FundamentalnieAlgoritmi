#ifndef ___MAIL_STRUCT___
#define ___MAIL_STRUCT___

#include "cstring.h"

typedef struct adress {
  string* city;
  string* street;
  unsigned long int house;
  unsigned long int flat;
  string* index;
} adress;

typedef struct mail {
  adress s_adress;
  adress r_adress;
  double weight;
  string* id;
  string* c_time;
  string* g_time;
} mail;

typedef struct post {
  adress* post_office;
  mail** mails;
} post;

#endif

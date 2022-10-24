#ifndef ___MAIL___
#define ___MAIL___

#include "cstring.h"
#include "cinteger.h"
#include "date.h"
#include "carray.h"
#include "mail_struct.h"

#define NO_SUCH_ELEMENT_EXCEPTION 9

void create_adress(adress* adr, string* city, string* street, unsigned long int house, unsigned long int flat, string* index);
void create_mail(mail* ml, string* city1, string* street1, unsigned long int house1, unsigned long int flat1, string* index1, string* city2, string* street2, unsigned long int house2, unsigned long int flat2, string* index2, double weight, string* id, string* c_time, string* g_time);
int mail_comporator(const void* cmp_lhs, const void* cmp_rhs);
int mail_delete_at(mail** mails, unsigned long int size, const string* id);
int mail_search_by_id(mail** mails, unsigned long int size, const string* id);
int mail_get_delivered(mail** mails, unsigned long int size, const string* time, mail*** result_mails, unsigned long int* deliver_ml_size);
int mail_date_comporator(const void* cmp_lhs, const void* cmp_rhs);
void print_all_mails(mail** ml, unsigned long int current_size);
int mail_get_expired(mail** mails, unsigned long int size, const string* time, mail*** result_mails, unsigned long int* expired_ml_size);
void free_mail(mail* ml);

#endif

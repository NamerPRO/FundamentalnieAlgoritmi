#include "mail.h"

void create_adress(adress* adr, string* city, string* street, unsigned long int house, unsigned long int flat, string* index) {
  adr->city = city;
  adr->street = street;
  adr->house = house;
  adr->flat = flat;
  adr->index = index;
}

void create_mail(mail* ml, string* city1, string* street1, unsigned long int house1, unsigned long int flat1, string* index1,
                  string* city2, string* street2, unsigned long int house2, unsigned long int flat2, string* index2,
                    double weight, string* id, string* c_time, string* g_time) {
  create_adress(&(ml->s_adress), city1, street1, house1, flat1, index1);
  create_adress(&(ml->r_adress), city2, street2, house2, flat2, index2);
  ml->weight = weight;
  ml->id = id;
  ml->c_time = c_time;
  ml->g_time = g_time;
}

int mail_comporator(const void* cmp_lhs, const void* cmp_rhs) {
  mail* lhs = *((mail**)cmp_lhs);
  mail* rhs = *((mail**)cmp_rhs);
  int cmp_res = string_compare("%s", standart_string_comporator, lhs->r_adress.index, rhs->r_adress.index);
  if (cmp_res != EQUALS) {
    return cmp_res;
  }
  return string_compare("%s", standart_string_comporator, lhs->id, rhs->id);
}

int mail_date_comporator(const void* cmp_lhs, const void* cmp_rhs) {
  mail* lhs = *((mail**)cmp_lhs);
  mail* rhs = *((mail**)cmp_rhs);
  int cmp_res = string_compare("%s", standart_date_comporator, lhs->c_time, rhs->c_time);
  if (cmp_res != EQUALS) {
    return cmp_res;
  }
  return string_compare("%s", standart_string_comporator, lhs->id, rhs->id);
}

int mail_delete_at(mail** mails, unsigned long int size, const string* id) {
  if (size <= 0) {
    return NO_SUCH_ELEMENT_EXCEPTION;
  }
  unsigned long int i = 0;
  for (; i < size; ++i) {
    if (!string_compare("%s", standart_string_comporator, mails[i]->id, id)) {
      break;
    }
  }
  if (i == size) {
    return NO_SUCH_ELEMENT_EXCEPTION;
  }
  free_mail(mails[i]);
  mails[i] = mails[size - 1];
  return SUCCESS_FUNCTION_RETURN;
}

void free_mail(mail* ml) {
  free_string(ml->s_adress.city);
  free(ml->s_adress.city);
  free_string(ml->s_adress.street);
  free(ml->s_adress.street);
  free_string(ml->s_adress.index);
  free(ml->s_adress.index);
  free_string(ml->r_adress.city);
  free(ml->r_adress.city);
  free_string(ml->r_adress.street);
  free(ml->r_adress.street);
  free_string(ml->r_adress.index);
  free(ml->r_adress.index);
  free_string(ml->id);
  free(ml->id);
  free_string(ml->c_time);
  free(ml->c_time);
  free_string(ml->g_time);
  free(ml->g_time);
}

void print_all_mails(mail** ml, unsigned long int current_size) {
  for (unsigned long int i = 0; i < current_size; ++i) {
    print_string(stdout, "N%d:\nSender adress=%s %s %d %d %s\nReceiver adress=%s %s %d %d %s\nweight=%f; id=%s; create time=%s; give time=%s\n", i + 1, ml[i]->s_adress.city, ml[i]->s_adress.street, ml[i]->s_adress.house, ml[i]->s_adress.flat, ml[i]->s_adress.index, ml[i]->r_adress.city, ml[i]->r_adress.street, ml[i]->r_adress.house, ml[i]->r_adress.flat, ml[i]->r_adress.index, ml[i]->weight, ml[i]->id, ml[i]->c_time, ml[i]->g_time);
  }
}

int mail_search_by_id(mail** mails, unsigned long int size, const string* id) {
  if (size <= 0) {
    return NO_SUCH_ELEMENT_EXCEPTION;
  }
  unsigned long int i = 0;
  for (; i < size; ++i) {
    if (!string_compare("%s", standart_string_comporator, mails[i]->id, id)) {
      break;
    }
  }
  if (i == size) {
    return NO_SUCH_ELEMENT_EXCEPTION;
  }
  print_string(stdout, "Sender adress=%s %s %d %d %s\nReceiver adress=%s %s %d %d %s\nweight=%f; id=%s; create time=%s; give time=%s\n", mails[i]->s_adress.city, mails[i]->s_adress.street, mails[i]->s_adress.house, mails[i]->s_adress.flat, mails[i]->s_adress.index, mails[i]->r_adress.city, mails[i]->r_adress.street, mails[i]->r_adress.house, mails[i]->r_adress.flat, mails[i]->r_adress.index, mails[i]->weight, mails[i]->id, mails[i]->c_time, mails[i]->g_time);
  return SUCCESS_FUNCTION_RETURN;
}

int mail_get_expired(mail** mails, unsigned long int size, const string* time, mail*** result_mails, unsigned long int* expired_ml_size) {
  mail_array m_arr;
  if (create_mail_array(&m_arr) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (unsigned long int i = 0; i < size; ++i) {
    int cmp_res = string_compare("%s", standart_date_comporator, mails[i]->g_time, time);
    if (cmp_res == FIRST_STRING) {
      if (insert_mail_in_array(&m_arr, mails[i]) == MEMORY_ALLOCATE_EXCEPTION) {
        for (unsigned long int i = 0; i < get_mail_array_size(&m_arr); ++i) {
          free_mail(get_mail_array(&m_arr)[i]);
        }
        free(get_mail_array(&m_arr));
        return MEMORY_ALLOCATE_EXCEPTION;
      }
    }
  }
  *result_mails = get_mail_array(&m_arr);
  *expired_ml_size = get_mail_array_size(&m_arr);
  return SUCCESS_FUNCTION_RETURN;
}

int mail_get_delivered(mail** mails, unsigned long int size, const string* time, mail*** result_mails, unsigned long int* deliver_ml_size) {
  mail_array m_arr;
  if (create_mail_array(&m_arr) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (unsigned long int i = 0; i < size; ++i) {
    int cmp_res = string_compare("%s", standart_date_comporator, mails[i]->g_time, time);
    if (cmp_res == EQUALS || cmp_res == SECOND_STRING) {
      if (insert_mail_in_array(&m_arr, mails[i]) == MEMORY_ALLOCATE_EXCEPTION) {
        for (unsigned long int i = 0; i < get_mail_array_size(&m_arr); ++i) {
          free_mail(get_mail_array(&m_arr)[i]);
        }
        free(get_mail_array(&m_arr));
        return MEMORY_ALLOCATE_EXCEPTION;
      }
    }
  }
  *result_mails = get_mail_array(&m_arr);
  *deliver_ml_size = get_mail_array_size(&m_arr);
  return SUCCESS_FUNCTION_RETURN;
}

#include <stdio.h>
#include <stdlib.h>

#include "cconstants.h"
#include "cstring.h"
#include "mail.h"
#include "cinteger.h"
#include "cio.h"
#include "date.h"

int space_cmp(int x) {
  if (x == '\n') {
    return 1;
  }
  return 0;
}

#define basic_free() \
          free_string(&time);\
          free_string(&action);\
          for (int i = 0; i < current_post_size; ++i) {\
            free_mail(pst.mails[i]);\
          }\
          free(pst.mails);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  string time;
  if (create_string(&time, argv[1]) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  int execute_status;
  unsigned long int successfully_read_variables;
  date dt;
  if ((execute_status = get_date(&time, &dt)) != SUCCESS_FUNCTION_RETURN) {
    free_string(&time);
    return execute_status;
  }
  int current_post_size = 0;
  int real_post_size = 10;
  post pst;
  if ((pst.mails = (mail**)malloc(sizeof(mail*) * real_post_size)) == NULL) {
    free_string(&time);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  print_string(stdout, "===\nActions list:\ncreate = add mail to post\nremove = remove mail in post\nfind = search by post id\ndeliver = get delivered on time mails\nexpired = get mails not delivered on time\nprint = display all mails\n===\n");
  string action;
  if (create_empty_string(&action) == MEMORY_ALLOCATE_EXCEPTION) {
    free_string(&time);
    free(pst.mails);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  while ((execute_status = read_string(stdin, &action, isspace)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      basic_free();
      return execute_status;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "create")) {
      print_string(stdout, "Fill sender adress (city, street, house, flat, index):\n");
      string* city1 = (string*)malloc(sizeof(string));
      if (city1 == NULL) {
        basic_free();
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      string* street1 = (string*)malloc(sizeof(string));
      if (street1 == NULL) {
        basic_free();
        free(city1);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      string* index1 = (string*)malloc(sizeof(string));
      if (index1 == NULL) {
        basic_free();
        free(city1);
        free(street1);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      int house1, flat1;
      if ((execute_status = safe_read(stdin, &successfully_read_variables, "%s%s%d%d%s", isspace, city1, street1, &house1, &flat1, index1)) != SUCCESS_FUNCTION_RETURN) {
        if (successfully_read_variables == 1) {
          free_string(city1);
        } else if (successfully_read_variables >= 2) {
          free_string(city1);
          free_string(street1);
        }
        basic_free();
        free(city1);
        free(street1);
        free(index1);
        return execute_status;
      }
      if (!string_contains_only(city1, standart_string_range) || !string_contains_only(street1, standart_string_range)
            || house1 <= 0 || flat1 <= 0 || !string_is_integer(index1) || get_string_size(index1) != 6) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free(city1);
        free(street1);
        free(index1);
        return WRONG_INPUT_EXCEPTION;
      }
      print_string(stdout, "Fill reciever adress (city, street, house, flat, index):\n");
      string* city2 = (string*)malloc(sizeof(string));
      if (city2 == NULL) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free(city1);
        free(street1);
        free(index1);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      string* street2 = (string*)malloc(sizeof(string));
      if (street2 == NULL) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      string* index2 = (string*)malloc(sizeof(string));
      if (index2 == NULL) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      int house2, flat2;
      if ((execute_status = safe_read(stdin, &successfully_read_variables, "%s%s%d%d%s", isspace, city2, street2, &house2, &flat2, index2)) != SUCCESS_FUNCTION_RETURN) {
        if (successfully_read_variables == 1) {
          free_string(city2);
        } else if (successfully_read_variables >= 2) {
          free_string(city2);
          free_string(street2);
        }
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        return execute_status;
      }
      if (!string_contains_only(city2, standart_string_range) || !string_contains_only(street2, standart_string_range)
            || house2 <= 0 || flat2 <= 0 || !string_is_integer(index2) || get_string_size(index2) != 6) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free_string(city2);
        free_string(street2);
        free_string(index2);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        return WRONG_INPUT_EXCEPTION;
      }
      double weight;
      print_string(stdout, "Enter weight:\n");
      scanf("%lf", &weight);
      if (weight < -STANDART_EPSIOLON) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free_string(city2);
        free_string(street2);
        free_string(index2);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        return WRONG_INPUT_EXCEPTION;
      }
      getchar();
      print_string(stdout, "Enter post identifier, send time and receive time via enter:\n");
      string* m_id = (string*)malloc(sizeof(string));
      if (m_id == NULL) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free_string(city2);
        free_string(street2);
        free_string(index2);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      string* c_time = (string*)malloc(sizeof(string));
      if (c_time == NULL) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free_string(city2);
        free_string(street2);
        free_string(index2);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        free(m_id);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      string* g_time = (string*)malloc(sizeof(string));
      if (g_time == NULL) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free_string(city2);
        free_string(street2);
        free_string(index2);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        free(m_id);
        free(c_time);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      if ((execute_status = safe_read(stdin, &successfully_read_variables, "%s%s%s", space_cmp, m_id, c_time, g_time)) != SUCCESS_FUNCTION_RETURN) {
        if (successfully_read_variables == 1) {
          free_string(m_id);
        } else if (successfully_read_variables == 2) {
          free_string(m_id);
          free_string(c_time);
        }
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free_string(city2);
        free_string(street2);
        free_string(index2);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        free(m_id);
        free(c_time);
        free(g_time);
        return execute_status;
      }
      if (!string_is_integer(m_id) || get_string_size(m_id) != 14) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free_string(city2);
        free_string(street2);
        free_string(index2);
        free_string(m_id);
        free_string(c_time);
        free_string(g_time);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        free(m_id);
        free(c_time);
        free(g_time);
        return WRONG_INPUT_EXCEPTION;
      }
      if ((execute_status = get_date(c_time, &dt)) != SUCCESS_FUNCTION_RETURN
            || (execute_status = get_date(g_time, &dt)) != SUCCESS_FUNCTION_RETURN) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free_string(city2);
        free_string(street2);
        free_string(index2);
        free_string(m_id);
        free_string(c_time);
        free_string(g_time);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        free(m_id);
        free(c_time);
        free(g_time);
        return execute_status;
      }
      mail* ml = (mail*)malloc(sizeof(mail));
      if (ml == NULL) {
        basic_free();
        free_string(city1);
        free_string(street1);
        free_string(index1);
        free_string(city2);
        free_string(street2);
        free_string(index2);
        free_string(m_id);
        free_string(c_time);
        free_string(g_time);
        free(city1);
        free(street1);
        free(index1);
        free(city2);
        free(street2);
        free(index2);
        free(m_id);
        free(c_time);
        free(g_time);
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      create_mail(ml, city1, street1, house1, flat1, index1, city2, street2, house2, flat2, index2, weight, m_id, c_time, g_time);
      if (current_post_size == real_post_size - 1) {
        mail** possible_mails = (mail**)realloc(pst.mails, sizeof(mail*) * 2 * real_post_size);
        if (possible_mails == NULL) {
          basic_free();
          free_mail(ml);
          free(ml);
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        pst.mails = possible_mails;
        real_post_size *= 2;
      }
      pst.mails[current_post_size++] = ml;
      qsort(pst.mails, current_post_size, sizeof(mail*), mail_comporator);
      print_string(stdout, "Mail was successfully created!\n");
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "remove")) {
      print_string(stdout, "Enter mail identifier:\n");
      string mail_id;
      if ((execute_status = safe_read(stdin, &successfully_read_variables, "%s", isspace, &mail_id)) != SUCCESS_FUNCTION_RETURN) {
        basic_free();
        return execute_status;
      }
      if (mail_delete_at(pst.mails, current_post_size, &mail_id) == NO_SUCH_ELEMENT_EXCEPTION) {
        print_string(stdout, "Seems like you try to delete element that does not exist or there was an internal error!\n");
      } else {
        --current_post_size;
        qsort(pst.mails, current_post_size, sizeof(mail*), mail_comporator);
        print_string(stdout, "Mail was successfully removed!\n");
      }
      free_string(&mail_id);
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "find")) {
      print_string(stdout, "Enter mail identifier:\n");
      string mail_id;
      if ((execute_status = safe_read(stdin, &successfully_read_variables, "%s", isspace, &mail_id)) != SUCCESS_FUNCTION_RETURN) {
        basic_free();
        return execute_status;
      }
      if (mail_search_by_id(pst.mails, current_post_size, &mail_id) == NO_SUCH_ELEMENT_EXCEPTION) {
        print_string(stdout, "No data found about such id!\n");
      }
      free_string(&mail_id);
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "deliver")) {
      mail** deliver_ml;
      unsigned long int deliver_ml_size = 0;
      if (mail_get_delivered(pst.mails, current_post_size, &time, &deliver_ml, &deliver_ml_size) == MEMORY_ALLOCATE_EXCEPTION) {
        basic_free();
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      qsort(deliver_ml, deliver_ml_size, sizeof(mail*), mail_date_comporator);
      print_all_mails(deliver_ml, deliver_ml_size);
      free(deliver_ml);
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "expired")) {
      mail** expired_ml;
      unsigned long int expired_ml_size = 0;
      if (mail_get_expired(pst.mails, current_post_size, &time, &expired_ml, &expired_ml_size) == MEMORY_ALLOCATE_EXCEPTION) {
        basic_free();
        return MEMORY_ALLOCATE_EXCEPTION;
      }
      qsort(expired_ml, expired_ml_size, sizeof(mail*), mail_date_comporator);
      print_all_mails(expired_ml, expired_ml_size);
      free(expired_ml);
      continue;
    }
    if (!string_compare("%p", standart_string_comporator, &action, "print")) {
      print_all_mails(pst.mails, current_post_size);
      continue;
    }
    print_string(stdout, "Unknown action was entered! Use create or remove.\n");
  }
  basic_free();
  return 0;
}

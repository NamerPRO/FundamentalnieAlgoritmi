#include <stdio.h>

#ifdef ___UNIX___

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#endif

#ifdef ___WINDOWS___

#include <Windows.h>

#endif

#include "cstring.h"
#include "cconstants.h"
#include "cserializer.h"
#include "csemophore.h"
#include "cpolymath.h"

int id_ctl(int flag, ...) {
  int sem_value;
  switch (flag) {
    case ID_FREE: ;
      va_list args;
      va_start(args, flag);

      #ifdef ___UNIX___

      if ((sem_value = semctl(sem_ids, 0, GETVAL)) == -1) {
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      #ifdef ___WINDOWS___

      get_windows_semophore_value(&sem_value, sem_ids);

      #endif

      ids[sem_value] = va_arg(args, int);
      va_end(args);

      #ifdef ___UNIX___

      struct sembuf inc[] = { { .sem_num = 0, .sem_op = 1, .sem_flg = 0 } };
      if (semop(sem_ids, inc, 1) == -1) {
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      #ifdef ___WINDOWS___

      if (ReleaseSemaphore(sem_ids, 1, NULL) == 0) {
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      return 0;
    case ID_GET: ;
      #ifdef ___UNIX___

      struct sembuf dec[] = { { .sem_num = 0, .sem_op = -1, .sem_flg = 0 } };
      if (semop(sem_ids, dec, 1) == -1) {
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }
      if ((sem_value = semctl(sem_ids, 0, GETVAL)) == -1) {
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      #ifdef ___WINDOWS___

      if (WaitForSingleObject(sem_ids, INFINITE) != 0) {
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }
      get_windows_semophore_value(&sem_value, sem_ids);

      #endif

      return ids[sem_value];
  }
  return -1;
}

int main() {
  int execute_status;
  if ((execute_status = create_serialized_string_array(&requests_storage, IDS_AMOUNT)) != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  response_storage = (polynomial**)malloc(sizeof(polynomial*) * IDS_AMOUNT);
  if (response_storage == NULL) {
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (int i = 0; i < IDS_AMOUNT; ++i) {
    response_storage[i] = NULL;
  }
  is_result_new = (int*)calloc(IDS_AMOUNT, sizeof(int));
  if (is_result_new == NULL) {
    free(response_storage);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  union semun sem_union;

  #ifdef ___UNIX___

  sem_ids = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
  sem_union.val = IDS_AMOUNT;
  if (semctl(sem_ids, 0, SETVAL, sem_union) == -1) {
    free(response_storage);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    semctl(sem_ids, 0, IPC_RMID);
    return SEMOPHORE_INTERRACT_EXCEPTION;
  }

  #endif

  #ifdef ___WINDOWS___

  sem_ids = CreateSemaphore(NULL, 999, 999, "d");
  if (sem_ids == 0) {
    free(response_storage);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    return SEMOPHORE_INTERRACT_EXCEPTION;
  }

  #endif

  for (int i = 0; i < IDS_AMOUNT; ++i) {
    ids[i] = i;
  }
  is_result_new = (int*)calloc(IDS_AMOUNT, sizeof(int));
  if (is_result_new == NULL) {
    free(response_storage);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);

    #ifdef ___UNIX___

    semctl(sem_ids, 0, IPC_RMID);

    #endif

    #ifdef ___WINDOWS___

    CloseHandle(sem_ids);

    #endif

    return MEMORY_ALLOCATE_EXCEPTION;
  }

  #ifdef ___UNIX___

  int client_and_server;
  if ((execute_status = set_semophores_to_default_unix(&client_and_server)) != SUCCESS_FUNCTION_RETURN) {
    free(response_storage);
    free(is_result_new);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    semctl(sem_ids, 0, IPC_RMID);
    return execute_status;
  }
  int desc = open("./data_file", O_RDWR | O_CREAT, 0600);
  if (desc == -1) {
    free(response_storage);
    free(is_result_new);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    semctl(sem_ids, 0, IPC_RMID);
    remove_default_semophores(client_and_server);
    return FILE_INTERRACT_EXCEPTION;
  }
  if (ftruncate(desc, STANDART_MAPPED_FILE_SIZE) == -1) {
    free(response_storage);
    free(is_result_new);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    semctl(sem_ids, 0, IPC_RMID);
    remove_default_semophores(client_and_server);
    return FILE_INTERRACT_EXCEPTION;
  }
  char* map = mmap(0, STANDART_MAPPED_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
  if (map == MAP_FAILED) {
    free(response_storage);
    free(is_result_new);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    semctl(sem_ids, 0, IPC_RMID);
    remove_default_semophores(client_and_server);
    return MAP_INTERRACT_EXCEPTION;
  }
  struct sembuf inc0[] = { { .sem_num = 0, .sem_op = 1, .sem_flg = 0 } };
  struct sembuf dec1[] = { { .sem_num = 1, .sem_op = -1, .sem_flg = 0 } };
  struct sembuf inc2[] = { { .sem_num = 2, .sem_op = 1, .sem_flg = 0 } };

  #endif

  #ifdef ___WINDOWS___

  int* client_and_server;
  if ((execute_status = set_semophores_to_default_windows(&client_and_server)) != SUCCESS_FUNCTION_RETURN) {
    free(response_storage);
    free(is_result_new);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    CloseHandle(sem_ids);
    return execute_status;
  }

  int desc = CreateFile("./data_file", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
  if (desc == INVALID_HANDLE_VALUE) {
    free(response_storage);
    free(is_result_new);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    CloseHandle(sem_ids);
    remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
    return FILE_INTERRACT_EXCEPTION;
  }

  int pre_map = CreateFileMapping(desc, NULL, PAGE_READWRITE, 0, STANDART_MAPPED_FILE_SIZE, "map");
  if (pre_map == NULL) {
    free(response_storage);
    free(is_result_new);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    CloseHandle(sem_ids);
    remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
    return MAP_INTERRACT_EXCEPTION;
  }

  char* map = MapViewOfFile(pre_map, FILE_MAP_ALL_ACCESS, 0, 0, STANDART_MAPPED_FILE_SIZE);
  if (map == NULL) {
    free(response_storage);
    free(is_result_new);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);
    CloseHandle(sem_ids);
    remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
    return MAP_INTERRACT_EXCEPTION;
  }

  #endif

  serialized_string* com_string = create_serialized_string(EMPTY);
  if (com_string == NULL) {
    free(response_storage);
    free(is_result_new);
    free_serialized_string_array(&requests_storage, IDS_AMOUNT);

    #ifdef ___UNIX___

    semctl(sem_ids, 0, IPC_RMID);

    #endif

    #ifdef ___WINDOWS___

    CloseHandle(sem_ids);

    #endif

    remove_default_semophores(client_and_server);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  int index;
  while (1) {

    #ifdef ___UNIX___

    if (semop(client_and_server, dec1, 1) == -1) {
      free(response_storage);
      free(is_result_new);
      free_serialized_string_array(&requests_storage, IDS_AMOUNT);
      semctl(sem_ids, 0, IPC_RMID);
      remove_default_semophores(client_and_server);
      free_serialized_string(com_string);
      return SEMOPHORE_INTERRACT_EXCEPTION;
    }

    #endif

    #ifdef ___WINDOWS___

    if (WaitForSingleObject(client_and_server[1], INFINITE) != 0) {
      free(response_storage);
      free(is_result_new);
      free_serialized_string_array(&requests_storage, IDS_AMOUNT);
      CloseHandle(sem_ids);
      remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
      free_serialized_string(com_string);
      return SEMOPHORE_INTERRACT_EXCEPTION;
    }

    #endif

    index = 0;
    flush_serialized_string(com_string, map, STANDART_MAPPED_FILE_SIZE);
    char action = deserialize_char(com_string, &index);
    int guid = deserialize_integer(com_string, &index);
    int return_guid = 0;
    if (guid == -1) {
      return_guid = 1;
      guid = id_ctl(ID_GET);
    }
    switch (action) {
      case 'a':
        pour_in_serialized_string(requests_storage[guid], com_string, index);
        break;
      case 'f':
        pour_in_serialized_string(requests_storage[guid], com_string, index);
        index = 0;
        struct dereq* de_request = deserialize_request(requests_storage[guid], &index);
        if (de_request == NULL) {
          free(response_storage);
          free(is_result_new);
          free_serialized_string_array(&requests_storage, IDS_AMOUNT);

          #ifdef ___UNIX___

          semctl(sem_ids, 0, IPC_RMID);

          #endif

          #ifdef ___WINDOWS___

          CloseHandle(sem_ids);

          #endif

          remove_default_semophores(client_and_server);
          free_serialized_string(com_string);
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        flush_serialized_string(requests_storage[guid], NULL);
        polynomial* first = (de_request->amount <= 1) ? NULL : de_request->polynomials[1];

        #ifdef ___UNIX___

        pthread_t thr;

        #endif

        #ifdef ___WINDOWS___

        int thr;

        #endif

        if (!strcmp(de_request->instruction, "Add")) {
          struct thrd_dt data = { .poly1 = first, .second.poly2 = de_request->polynomials[0], .id = guid };

          #ifdef ___UNIX___

          if (pthread_create(&thr, NULL, add, &data) != 0) {
            free(response_storage);
            free(is_result_new);
            free_serialized_string_array(&requests_storage, IDS_AMOUNT);
            semctl(sem_ids, 0, IPC_RMID);
            remove_default_semophores(client_and_server);
            free_serialized_string(com_string);
          }

          #endif

          #ifdef ___WINDOWS___

          if (CreateThread(NULL, 0, add, &data, 0, NULL) == 0) {
            free(response_storage);
            free(is_result_new);
            free_serialized_string_array(&requests_storage, IDS_AMOUNT);
            CloseHandle(sem_ids);
            remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
            free_serialized_string(com_string);
          }

          #endif

        } else if (!strcmp(de_request->instruction, "Sub")) {
          struct thrd_dt data = { .poly1 = de_request->polynomials[0], .second.poly2 = first, .id = guid };

          #ifdef ___UNIX___

          if (pthread_create(&thr, NULL, sub, &data) != 0) {
            free(response_storage);
            free(is_result_new);
            free_serialized_string_array(&requests_storage, IDS_AMOUNT);
            semctl(sem_ids, 0, IPC_RMID);
            remove_default_semophores(client_and_server);
            free_serialized_string(com_string);
          }

          #endif

          #ifdef ___WINDOWS___

          if (CreateThread(NULL, 0, sub, &data, 0, NULL) == 0) {
            free(response_storage);
            free(is_result_new);
            free_serialized_string_array(&requests_storage, IDS_AMOUNT);
            CloseHandle(sem_ids);
            remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
            free_serialized_string(com_string);
          }

          #endif

        } else if (!strcmp(de_request->instruction, "Mult")) {
          struct thrd_dt data = { .poly1 = first, .second.poly2 = de_request->polynomials[0], .id = guid };

          #ifdef ___UNIX___

          if (pthread_create(&thr, NULL, mult, &data) != 0) {
            free(response_storage);
            free(is_result_new);
            free_serialized_string_array(&requests_storage, IDS_AMOUNT);
            semctl(sem_ids, 0, IPC_RMID);
            remove_default_semophores(client_and_server);
            free_serialized_string(com_string);
          }

          #endif
          #ifdef ___WINDOWS___

          if (CreateThread(NULL, 0, mult, &data, 0, NULL) == 0) {
            free(response_storage);
            free(is_result_new);
            free_serialized_string_array(&requests_storage, IDS_AMOUNT);
            CloseHandle(sem_ids);
            remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
            free_serialized_string(com_string);
          }

          #endif

        } else if (!strcmp(de_request->instruction, "Eval")) {
          if (de_request->amount == 0) {
            struct thrd_dt data = { .poly1 = NULL, .second.dots_data = { .count = de_request->dot->count, .coordinates = de_request->dot->coordinates }, .id = guid };

            #ifdef ___UNIX___

            if (pthread_create(&thr, NULL, eval, &data) != 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              semctl(sem_ids, 0, IPC_RMID);
              remove_default_semophores(client_and_server);
              free_serialized_string(com_string);
            }

            #endif

            #ifdef ___WINDOWS___

            if (CreateThread(NULL, 0, eval, &data, 0, NULL) == 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              CloseHandle(sem_ids);
              remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
              free_serialized_string(com_string);
            }

            #endif

          } else {
            struct thrd_dt data = { .poly1 = de_request->polynomials[0], .second.dots_data = { .count = de_request->dot->count, .coordinates = de_request->dot->coordinates }, .id = guid };

            #ifdef ___UNIX___

            if (pthread_create(&thr, NULL, eval, &data) != 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              semctl(sem_ids, 0, IPC_RMID);
              remove_default_semophores(client_and_server);
              free_serialized_string(com_string);
            }

            #endif

            #ifdef ___WINDOWS___

            if (CreateThread(NULL, 0, eval, &data, 0, NULL) == 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              CloseHandle(sem_ids);
              remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
              free_serialized_string(com_string);
            }

            #endif

          }
        } else if (!strcmp(de_request->instruction, "Diff")) {
          if (first == NULL) {
            char* variable = de_request->polynomials[0]->monomials->head->value->unknowns[0]->name;
            struct thrd_dt data = { .poly1 = NULL, .second.var = variable, .id = guid };

            #ifdef ___UNIX___

            if (pthread_create(&thr, NULL, diff, &data) != 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              semctl(sem_ids, 0, IPC_RMID);
              remove_default_semophores(client_and_server);
              free_serialized_string(com_string);
            }

            #endif

            #ifdef ___WINDOWS___

            if (CreateThread(NULL, 0, diff, &data, 0, NULL) == 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              CloseHandle(sem_ids);
              remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
              free_serialized_string(com_string);
            }

            #endif

          } else {
            char* variable = first->monomials->head->value->unknowns[0]->name;
            struct thrd_dt data = { .poly1 = de_request->polynomials[0], .second.var = variable, .id = guid };

            #ifdef ___UNIX___

            if (pthread_create(&thr, NULL, diff, &data) != 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              semctl(sem_ids, 0, IPC_RMID);
              remove_default_semophores(client_and_server);
              free_serialized_string(com_string);
            }

            #endif

            #ifdef ___WINDOWS___

            if (CreateThread(NULL, 0, diff, &data, 0, NULL) == 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              CloseHandle(sem_ids);
              remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
              free_serialized_string(com_string);
            }

            #endif

          }
        } else if (!strcmp(de_request->instruction, "Grad")) {
          if (de_request->amount == 0) {
            struct thrd_dt data = { .poly1 = NULL, .second.poly2 = NULL, .id = guid };

            #ifdef ___UNIX___

            if (pthread_create(&thr, NULL, grad, &data) != 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              semctl(sem_ids, 0, IPC_RMID);
              remove_default_semophores(client_and_server);
              free_serialized_string(com_string);
            }

            #endif

            #ifdef ___WINDOWS___

            if (CreateThread(NULL, 0, grad, &data, 0, NULL) == 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              CloseHandle(sem_ids);
              remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
              free_serialized_string(com_string);
            }

            #endif

          } else {
            struct thrd_dt data = { .poly1 = de_request->polynomials[0], .second.poly2 = NULL, .id = guid };

            #ifdef ___UNIX___

            if (pthread_create(&thr, NULL, grad, &data) != 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              semctl(sem_ids, 0, IPC_RMID);
              remove_default_semophores(client_and_server);
              free_serialized_string(com_string);
            }

            #endif

            #ifdef ___WINDOWS___

            if (CreateThread(NULL, 0, grad, &data, 0, NULL) == 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              CloseHandle(sem_ids);
              remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
              free_serialized_string(com_string);
            }

            #endif
          }
        } else if (!strcmp(de_request->instruction, "Print")) {
          if (de_request->amount == 0) {
            struct thrd_dt data = { .poly1 = NULL, .second.poly2 = NULL, .id = guid };

            #ifdef ___UNIX___

            if (pthread_create(&thr, NULL, print, &data) != 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              semctl(sem_ids, 0, IPC_RMID);
              remove_default_semophores(client_and_server);
              free_serialized_string(com_string);
            }

            #endif

            #ifdef ___WINDOWS___

            if (CreateThread(NULL, 0, print, &data, 0, NULL) == 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              CloseHandle(sem_ids);
              remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
              free_serialized_string(com_string);
            }

            #endif

          } else {
            struct thrd_dt data = { .poly1 = de_request->polynomials[0], .second.poly2 = NULL, .id = guid };

            #ifdef ___UNIX___

            if (pthread_create(&thr, NULL, print, &data) != 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              semctl(sem_ids, 0, IPC_RMID);
              remove_default_semophores(client_and_server);
              free_serialized_string(com_string);
            }

            #endif

            #ifdef ___WINDOWS___

            if (CreateThread(NULL, 0, print, &data, 0, NULL) == 0) {
              free(response_storage);
              free(is_result_new);
              free_serialized_string_array(&requests_storage, IDS_AMOUNT);
              CloseHandle(sem_ids);
              remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
              free_serialized_string(com_string);
            }

            #endif

          }
        }
        break;
      case 'r':
        if (!is_result_new[guid]) {
          map[0] = 'n';
        } else {
          is_result_new[guid] = 0;
          map[0] = 'y';
        }
        break;
      case 's':
        free_polynomial(response_storage[guid]);
        response_storage[guid] = NULL;
        id_ctl(ID_FREE, guid);
        break;
    }
    if (return_guid) {
      flush_serialized_string(com_string, NULL);
      serialize_integer(guid, com_string);
      int resp_size = get_serialized_string_size(com_string);
      memcpy(map, get_raw_serialized_string(com_string), resp_size);
    }

    #ifdef ___UNIX___

    if (semop(client_and_server, inc2, 1) == -1) {
      free(response_storage);
      free(is_result_new);
      free_serialized_string_array(&requests_storage, IDS_AMOUNT);
      semctl(sem_ids, 0, IPC_RMID);
      remove_default_semophores(client_and_server);
      free_serialized_string(com_string);
      return SEMOPHORE_INTERRACT_EXCEPTION;
    }

    #endif

    #ifdef ___WINDOWS___

    if (ReleaseSemaphore(client_and_server[2], 1, NULL) == 0) {
      free(response_storage);
      free(is_result_new);
      free_serialized_string_array(&requests_storage, IDS_AMOUNT);
      CloseHandle(sem_ids);
      remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);
      free_serialized_string(com_string);
    }

    #endif
  }
  free(response_storage);
  free(is_result_new);
  free_serialized_string_array(&requests_storage, IDS_AMOUNT);

  #ifdef ___UNIX___

  semctl(sem_ids, 0, IPC_RMID);
  remove_default_semophores(client_and_server);

  #endif

  #ifdef ___WINDOWS___

  CloseHandle(sem_ids);
  remove_default_semophores(client_and_server[0], client_and_server[1], client_and_server[2]);

  #endif

  free_serialized_string(com_string);
  return 0;
}

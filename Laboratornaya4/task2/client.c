#include <stdio.h>
#include <string.h>

#ifdef ___UNIX___

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/io.h>

#endif

#ifdef ___WINDOWS___

#include <Windows.h>

#endif

#include "cstring.h"
#include "csemophore.h"
#include "cserializer.h"
#include "cpolynomial.h"

#define READ 1
#define LINE_COMMENT 2
#define MULTILINE_COMMENT 3

enum states {
  SEND,
  PING
};

int read_instruction(int symbol, int index) {
  static int flag = READ;
  if ((symbol == '\n' && flag != LINE_COMMENT) || symbol == ' ') {
    return SKIP_CHARACTER;
  }
  if (symbol == ';' && flag == READ) {
    return STOP_AND_INCLUDE;
  }
  switch (flag) {
    case LINE_COMMENT:
      if (symbol == '\n') {
        flag = READ;
      }
      return SKIP_CHARACTER;
    case MULTILINE_COMMENT:
      if (symbol == ']') {
        flag = READ;
      }
      return SKIP_CHARACTER;
    case READ:
      if (symbol  == '%') {
        flag = LINE_COMMENT;
        return SKIP_CHARACTER;
      } else if (symbol == '[') {
        flag = MULTILINE_COMMENT;
        return SKIP_CHARACTER;
      }
      return INCLUDE_CHARACTER;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  string* line = heap_string(EMPTY);
  if (line == NULL) {
    fclose(file);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  int execute_status;
  serialized_string* com_string = create_serialized_string(EMPTY);
  if (com_string == NULL) {
    free_heap_string(line);
    fclose(file);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  int guid = -1;
  serialized_string* com_guid = create_serialized_string(EMPTY);
  if (com_guid == NULL) {
    free_serialized_string(com_string);
    free_heap_string(line);
    fclose(file);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  if (serialize_integer(guid, com_guid) == MEMORY_ALLOCATE_EXCEPTION) {
    free_serialized_string(com_string);
    free_serialized_string(com_guid);
    free_heap_string(line);
    fclose(file);
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  int guid_size = get_serialized_string_size(com_guid);

  #ifdef ___UNIX___

  int desc = open("./data_file", O_RDWR | O_CREAT, 0600);
  if (desc == -1) {
    free_serialized_string(com_guid);
    free_serialized_string(com_string);
    free_heap_string(line);
    fclose(file);
    return FILE_INTERRACT_EXCEPTION;
  }
  char* map = (char*)mmap(0, STANDART_MAPPED_FILE_SIZE, PROT_WRITE, MAP_SHARED, desc, 0);
  if (map == MAP_FAILED) {
    free_serialized_string(com_guid);
    free_serialized_string(com_string);
    free_heap_string(line);
    fclose(file);
    close(desc);
    return MAP_INTERRACT_EXCEPTION;
  }
  int client_and_server = semget(ftok("./server.c", PROJECT_ID), 3, 0600);
  if (client_and_server == -1) {
    munmap(map, STANDART_MAPPED_FILE_SIZE);
    free_serialized_string(com_guid);
    free_serialized_string(com_string);
    free_heap_string(line);
    fclose(file);
    close(desc);
    return SEMOPHORE_INTERRACT_EXCEPTION;
  }
  struct sembuf inc0[] = { { .sem_num = 0, .sem_op = 1, .sem_flg = 0 } };
  struct sembuf dec0[] = { { .sem_num = 0, .sem_op = -1, .sem_flg = 0 } };
  struct sembuf dec2[] = { { .sem_num = 2, .sem_op = -1, .sem_flg = 0 } };
  struct sembuf inc1[] = { { .sem_num = 1, .sem_op = 1, .sem_flg = 0 } };

  #endif

  #ifdef ___WINDOWS___

  int* client_and_server;
  if ((execute_status = set_semophores_to_default_windows(&client_and_server)) != SUCCESS_FUNCTION_RETURN) {
    free_serialized_string(com_guid);
    free_serialized_string(com_string);
    free_heap_string(line);
    fclose(file);
    return execute_status;
  }

  int desc = CreateFile("./data_file", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
  if (desc == INVALID_HANDLE_VALUE) {
    free_serialized_string(com_guid);
    free_serialized_string(com_string);
    free_heap_string(line);
    fclose(file);
    CloseHandle(desc);
    return FILE_INTERRACT_EXCEPTION;
  }

  int pre_map = CreateFileMapping(desc, NULL, PAGE_READWRITE, 0, STANDART_MAPPED_FILE_SIZE, "map");
  if (pre_map == NULL) {
    free_serialized_string(com_guid);
    free_serialized_string(com_string);
    free_heap_string(line);
    fclose(file);
    CloseHandle(desc);
    return MAP_INTERRACT_EXCEPTION;
  }

  char* map = MapViewOfFile(pre_map, FILE_MAP_ALL_ACCESS, 0, 0, STANDART_MAPPED_FILE_SIZE);
  if (map == NULL) {
    free_serialized_string(com_guid);
    free_serialized_string(com_string);
    free_heap_string(line);
    fclose(file);
    CloseHandle(desc);
    return MAP_INTERRACT_EXCEPTION;
  }

  #endif

  enum states state = SEND;
  while (1) {
    if (state == SEND) {

      #ifdef ___UNIX___

      if (semop(client_and_server, dec0, 1) == -1) {
        set_semophores_to_default_unix(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        close(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      #ifdef ___WINDOWS___

      if (WaitForSingleObject(client_and_server[0], 0) != 0) {
        set_semophores_to_default_windows(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        CloseHandle(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      if ((execute_status = read_string(file, line, read_instruction)) == EOF) {
        map[0] = 's';
        memcpy(map + 1, get_raw_serialized_string(com_guid), guid_size);

        #ifdef ___UNIX___

        if (semop(client_and_server, inc0, 1) == -1) {
          set_semophores_to_default_unix(NULL);
          free_serialized_string(com_guid);
          free_serialized_string(com_string);
          free_heap_string(line);
          fclose(file);
          close(desc);
          return SEMOPHORE_INTERRACT_EXCEPTION;
        }

        #endif

        #ifdef ___WINDOWS___

        if (ReleaseSemaphore(client_and_server[0], 1, NULL) == 0) {
          set_semophores_to_default_windows(NULL);
          free_serialized_string(com_guid);
          free_serialized_string(com_string);
          free_heap_string(line);
          fclose(file);
          CloseHandle(desc);
          return SEMOPHORE_INTERRACT_EXCEPTION;
        }

        #endif

        break;
      }
      flush_serialized_string(com_string, NULL);
      if ((execute_status = serialize_request(com_string, line)) != SUCCESS_FUNCTION_RETURN) {
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);

        #ifdef ___UNIX___

        close(desc);
        // munmap(map, STANDART_MAPPED_FILE_SIZE);

        #endif

        #ifdef ___WINDOWS___

        CloseHandle(desc);

        #endif

        return execute_status;
      }
      char* msg = get_raw_serialized_string(com_string);
      int msg_size = get_serialized_string_size(com_string);
      #ifdef ___DEBUG_CLIENT___

      int inni = 0;
      print_string(stdout, "%s\n%d\n", line, msg_size);
      struct dereq* de_request = deserialize_request(com_string, &inni);
      print_deserialized_request(de_request);
      printf("\n");

      #endif

      while (msg_size > 0) {
        if (msg_size > STANDART_MAPPED_FILE_SIZE - 1 - guid_size) {
          map[0] = 'a';
          memcpy(map + 1, get_raw_serialized_string(com_guid), guid_size);
          memcpy(map + 1 + guid_size, msg, STANDART_MAPPED_FILE_SIZE - 1 - guid_size);
          msg += STANDART_MAPPED_FILE_SIZE - 1 - guid_size;
          msg_size -= STANDART_MAPPED_FILE_SIZE - 1 - guid_size;
        } else {
          map[0] = 'f';
          memcpy(map + 1, get_raw_serialized_string(com_guid), guid_size);
          memcpy(map + 1 + guid_size, msg, msg_size);
          msg_size = 0;
          state = PING;
        }

        #ifdef ___UNIX___

        if (semop(client_and_server, inc1, 1) == -1) {
          set_semophores_to_default_unix(NULL);
          free_serialized_string(com_guid);
          free_serialized_string(com_string);
          free_heap_string(line);
          fclose(file);
          close(desc);
          return SEMOPHORE_INTERRACT_EXCEPTION;
        }
        if (semop(client_and_server, dec2, 1) == -1) {
          set_semophores_to_default_unix(NULL);
          free_serialized_string(com_guid);
          free_serialized_string(com_string);
          free_heap_string(line);
          fclose(file);
          close(desc);
          return SEMOPHORE_INTERRACT_EXCEPTION;
        }

        #endif

        #ifdef ___WINDOWS___

        if (ReleaseSemaphore(client_and_server[1], 1, NULL) == 0) {
          set_semophores_to_default_windows(NULL);
          free_serialized_string(com_guid);
          free_serialized_string(com_string);
          free_heap_string(line);
          fclose(file);
          CloseHandle(desc);
          return SEMOPHORE_INTERRACT_EXCEPTION;
        }
        if (WaitForSingleObject(client_and_server[2], INFINITE) != 0) {
          set_semophores_to_default_windows(NULL);
          free_serialized_string(com_guid);
          free_serialized_string(com_string);
          free_heap_string(line);
          fclose(file);
          CloseHandle(desc);
          return SEMOPHORE_INTERRACT_EXCEPTION;
        }

        #endif

        if (guid == -1) {
          guid = 0;
          if (flush_serialized_string(com_guid, map, guid_size) == MEMORY_ALLOCATE_EXCEPTION) {
            free_serialized_string(com_guid);
            free_serialized_string(com_string);
            free_heap_string(line);
            fclose(file);

            #ifdef ___UNIX___

            set_semophores_to_default_unix(NULL);
            close(desc);

            #endif

            #ifdef ___WINDOWS___

            set_semophores_to_default_windows(NULL);
            CloseHandle(desc);

            #endif

            return MEMORY_ALLOCATE_EXCEPTION;
          }
        }
      }

      #ifdef ___UNIX___

      if (semop(client_and_server, inc0, 1) == -1) {
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        set_semophores_to_default_unix(NULL);
        close(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      #ifdef ___WINDOWS___

      if (ReleaseSemaphore(client_and_server[0], 1, NULL) == 0) {
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        set_semophores_to_default_windows(NULL);
        CloseHandle(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

    } else if (state == PING) {
      sleep(1);

      #ifdef ___UNIX___

      if (semop(client_and_server, dec0, 1) == -1) {
        set_semophores_to_default_unix(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        close(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      #ifdef ___WINDOWS___

      if (WaitForSingleObject(client_and_server[0], INFINITE) != 0) {
        set_semophores_to_default_windows(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        CloseHandle(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      map[0] = 'r';
      memcpy(map + 1, get_raw_serialized_string(com_guid), guid_size);

      #ifdef ___UNIX___

      if (semop(client_and_server, inc1, 1) == -1) {
        set_semophores_to_default_unix(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        close(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }
      if (semop(client_and_server, dec2, 1) == -1) {
        set_semophores_to_default_unix(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        close(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      #ifdef ___WINDOWS___

      if (ReleaseSemaphore(client_and_server[1], 1, NULL) == 0) {
        set_semophores_to_default_windows(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        CloseHandle(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }
      if (WaitForSingleObject(client_and_server[2], INFINITE) != 0) {
        set_semophores_to_default_windows(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        CloseHandle(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      if (map[0] == 'y') {
        state = SEND;
      }

      #ifdef ___UNIX___

      if (semop(client_and_server, inc0, 1) == -1) {
        set_semophores_to_default_unix(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        close(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

      #ifdef ___WINDOWS___

      if (ReleaseSemaphore(client_and_server[0], 1, NULL) == 0) {
        set_semophores_to_default_windows(NULL);
        free_serialized_string(com_guid);
        free_serialized_string(com_string);
        free_heap_string(line);
        fclose(file);
        CloseHandle(desc);
        return SEMOPHORE_INTERRACT_EXCEPTION;
      }

      #endif

    }
  }
  free_serialized_string(com_guid);
  free_serialized_string(com_string);
  free_heap_string(line);
  fclose(file);

  #ifdef ___UNIX___

  close(desc);

  #endif

  #ifdef ___WINDOWS___

  CloseHandle(desc);

  #endif

  return 0;
}

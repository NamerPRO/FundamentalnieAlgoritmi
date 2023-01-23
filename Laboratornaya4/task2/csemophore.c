#include "csemophore.h"

#ifdef ___WINDOWS___

void get_windows_semophore_value(int* count, int sem) {
  if (WAIT_OBJECT_0 == WaitForSingleObject(sem, 0)) {
    ReleaseSemaphore(sem, 1, *count);
    return;
  }
  *count = -1;
}

#endif

void remove_default_semophores(int client_and_server, ...) {

  #ifdef ___UNIX___

  semctl(client_and_server, 0, IPC_RMID);
  semctl(client_and_server, 1, IPC_RMID);
  semctl(client_and_server, 2, IPC_RMID);

  #endif


  #ifdef ___WINDOWS___

  va_list args;
  va_start(args, client_and_server);
  int sem2 = va_arg(args, int);
  int sem3 = va_arg(args, int);
  va_end(args);
  CloseHandle(client_and_server);
  CloseHandle(sem2);
  CloseHandle(sem3);

  #endif
}

#ifdef ___WINDOWS___

int set_semophores_to_default_windows(int** sem) {
  int sem1 = CreateSemaphore(NULL, 1, 1, "a");
  if (sem1 == 0) {
    return SEMOPHORE_INTERRACT_EXCEPTION;
  }
  int sem2 = CreateSemaphore(NULL, 0, 1, "b");
  if (sem2 == 0) {
    return SEMOPHORE_INTERRACT_EXCEPTION;
  }
  int sem3 = CreateSemaphore(NULL, 0, 1, "c");
  if (sem3 == 0) {
    return SEMOPHORE_INTERRACT_EXCEPTION;
  }

  if (sem != NULL) {
    *sem = (int*)malloc(sizeof(int) * 3);
    if (*sem == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    (*sem)[0] = sem1;
    (*sem)[1] = sem2;
    (*sem)[2] = sem3;
  }
  return SUCCESS_FUNCTION_RETURN;
}

#endif

#ifdef ___UNIX___

int set_semophores_to_default_unix(int* csem) {
  int sem = semget(ftok("./server.c", PROJECT_ID), 3, IPC_CREAT | 0600);
  union semun sem_union;
  sem_union.val = 1;
  if (semctl(sem, 0, SETVAL, sem_union) == -1) {
    return SEMOPHORE_INTERRACT_EXCEPTION;
  }
  sem_union.val = 0;
  if (semctl(sem, 1, SETVAL, sem_union) == -1) {
    return SEMOPHORE_INTERRACT_EXCEPTION;
  }
  if (semctl(sem, 2, SETVAL, sem_union) == -1) {
    return SEMOPHORE_INTERRACT_EXCEPTION;
  }
  if (csem != NULL) {
    *csem = sem;
  }

  return SUCCESS_FUNCTION_RETURN;
}

#endif

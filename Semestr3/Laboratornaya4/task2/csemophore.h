#ifndef ___CSEMOPHORE___
#define ___CSEMOPHORE___

#ifdef ___UNIX___

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#endif

#ifdef ___WINDOWS___

#include <Windows.h>

#endif

#include "cpolynomial.h"
#include "cconstants.h"

#if !defined(__GNU_LIBRARY__) || defined(_SEM_SEMUN_UNDEFINED)
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short* array;
  struct seminfo* __buf;
};
#endif

#ifdef ___UNIX___

int set_semophores_to_default_unix(int* csem);

#endif

void remove_default_semophores(int client_and_server, ...);

#ifdef ___WINDOWS___

int set_semophores_to_default_windows(int** sem);
void get_windows_semophore_value(int* count, int sem);

#endif

#endif

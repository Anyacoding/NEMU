#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);

extern char **environ;
void call_main(uintptr_t *args) {
  char* sp = (char*)args;
  int argc = *(int*)sp;
  char** argv = (char**)(sp + sizeof(argc));
  // printf("args: %p  argc: %d  argv[0]: %s\n", args, argc, argv[0]);
  // assert(0);
  char* envp[] = { NULL };
  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}

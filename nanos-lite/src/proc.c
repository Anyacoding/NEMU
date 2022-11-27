#include <proc.h>
#include <am.h>
#include "/home/anya/ics2021/abstract-machine/am/include/arch/riscv32-nemu.h"

#define MAX_NR_PROC 4

void naive_uload(PCB* pcb, const char* filename);
void context_uload(PCB *pcb, const char* pathname, char* argv[], char* const envp[]);

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

// 内核线程上下文装载
void context_kload(PCB* pcb, void (*entry)(void *), void *arg) {
  Area kstack = { .start = (void*)pcb->stack, .end = (void*)(pcb->stack +  STACK_SIZE)};
  pcb->cp = kcontext(kstack, entry, arg);
}


void init_proc() {
  Log("Initializing processes...");
  char* argv[] = { "pal", NULL, NULL };
  
  context_uload(&pcb[0], "/bin/nterm", argv, NULL);
  context_kload(&pcb[1], hello_fun, (void*)1);
  // context_uload(&pcb[1], "/bin/hello", argv, NULL);
  switch_boot_pcb();
  // 利用内陷来触发内核线程和进程的的开启
  yield();
  // load program here
  naive_uload(NULL, "/bin/dummy");
}

Context* schedule(Context *prev) {
  current->cp = prev;
  // current = &pcb[1];
  current = current == &pcb[0] ? &pcb[1] : &pcb[0];
  Log("Jump to entry = %p", current->cp->mepc);
  // Log("pcb[0] to entry = %p", pcb[0].cp->mepc);
  // Log("pcb[1] to entry = %p", pcb[1].cp->mepc);
  // printf("&pcb[0]->cp: %p\n", (&pcb[0])->cp);
  // printf("&pcb[1]->cp: %p\n", (&pcb[1])->cp);
  return current->cp;
}

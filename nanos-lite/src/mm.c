#include <memory.h>
#include <stdlib.h>
#include "/home/anya/ics2021/nanos-lite/include/common.h"
#include "/home/anya/ics2021/nanos-lite/include/proc.h"
#include <stdio.h>

#define PTE_V 0x01
#define PTE_R 0x02
#define PTE_W 0x04
#define PTE_X 0x08
#define PTE_U 0x10
#define PTE_A 0x40
#define PTE_D 0x80

static void *pf = NULL;

void map(AddrSpace* as, void* va, void* pa, int prot);

// 保持4kb对齐
void* new_page(size_t nr_page) {
  pf += nr_page * 4096;
  memset(pf - nr_page * 4096, 0, nr_page * 4096);
  return pf - nr_page * 4096;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
  void* ret = new_page(n / 4096);
  return ret;
}
#endif

void free_page(void *p) {
  pf -= PGSIZE;
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  if (current->max_brk == 0) {
    current->max_brk = brk;
    return 0;
  }
  int prot = 0;
  prot |= PTE_R | PTE_W | PTE_X;
  for (; current->max_brk <= ROUNDUP(brk, PGSIZE); current->max_brk += PGSIZE) {
    void* pa = pg_alloc(PGSIZE);
    map(&current->as, (void*)current->max_brk, pa, prot);
  }
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);
#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}


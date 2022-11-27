#include <am.h>
#include <nemu.h>
#include <klib.h>
#include "/home/anya/ics2021/abstract-machine/am/include/arch/riscv32-nemu.h"

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
  uintptr_t mode = 1ul << (__riscv_xlen - 1);
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
  
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;
  // 页表地址  注意RISCV32采用的是二级页表，所以这里指向的是第一级页表，也就是页目录
  kas.ptr = pgalloc_f(PGSIZE);

  int prot = 0;
  prot |= PTE_R | PTE_W | PTE_X;

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, prot);
    }
  }

  set_satp(kas.ptr);
  
  vme_enable = 1;
  printf("虚拟化完成！\n");
  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
  set_satp(updir);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
  uint32_t vpn1 = (uint32_t)va >> 22 & 0x3ff;
  uint32_t vpn2 = (uint32_t)va >> 12 & 0x3ff;
  // 保证偏移量的单位是四个个字节，即一个页表项的大小
  uintptr_t* kp_dir = as->ptr;
  if (*(kp_dir + vpn1) == 0) {
    *(kp_dir + vpn1) = (uintptr_t)pgalloc_usr(PGSIZE);
  }

  uintptr_t* kp_table = (uintptr_t*)*(kp_dir + vpn1);

  if (*(kp_table + vpn2) == 0) {
    *(kp_table + vpn2) = (((uint32_t)pa >> 10) << 10) | PTE_V | PTE_R | PTE_W | PTE_X;
  }
  else {
    printf("已经映射过了\n");
    pgfree_usr(NULL);
  }
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
  Context* c = (Context*)kstack.end - sizeof(Context);
  c->mepc = (uintptr_t)entry;
  c->mstatus = 0x1880;
  c->mcause = 0xb;
  c->pdir = as->ptr;
  c->np = 1;
  return c;
}
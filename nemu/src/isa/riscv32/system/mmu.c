#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>
#include "/home/anya/ics2021/abstract-machine/am/src/riscv/riscv.h"

// 现在先不考虑type

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
  uint32_t vpn1 = (uint32_t)vaddr >> 22 & 0x3ff;
  uint32_t vpn2 = (uint32_t)vaddr >> 12 & 0x3ff;
  uint32_t offset = (uint32_t)vaddr & 0xfff;
  // 草，这里是硬件层，不能直接解引用了，需要paddr_read
  paddr_t pdir = cpu.csr.msatp << 12;
  assert(pdir);
  paddr_t ptable = paddr_read(pdir + vpn1 * 4, 4);
  assert(ptable);
  uint32_t pte = paddr_read(ptable + vpn2 * 4, 4);
  int valid = pte & PTE_V;
  assert(valid);
  paddr_t pa = ((pte >> 12) << 12) | offset;
  return pa;
}

int isa_mmu_check(vaddr_t vaddr, int len, int type) {
  uint32_t mode = cpu.csr.msatp & 0x80000000;
  if (mode == 0) 
    return MMU_DIRECT;
  else 
    return MMU_TRANSLATE;
}


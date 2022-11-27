#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  int check = isa_mmu_check(addr, len, 0);
  word_t ret = 0;
  if (check == MMU_DIRECT) {
    ret = paddr_read(addr, len);
  }
  else if (check == MMU_TRANSLATE) {
    ret = paddr_read(isa_mmu_translate(addr, len, 0), len);
  }
  return ret;
}

word_t vaddr_read(vaddr_t addr, int len) {
  return vaddr_ifetch(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  int check = isa_mmu_check(addr, len, 0);
  if (check == MMU_DIRECT) {
    paddr_write(addr, len, data);
  }
  else if (check == MMU_TRANSLATE) {
    paddr_write(isa_mmu_translate(addr, len, 0), len, data);
  }
}

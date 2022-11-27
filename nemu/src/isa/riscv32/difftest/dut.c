#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if (cpu.pc != ref_r->pc) {
    printf("pc值出错\n");
    printf("ref的pc值: %#x\n", ref_r->pc);
    printf("cpu的pc值: %#x\n", cpu.pc);
    return false;
  }
  for (int i = 0; i < 32; ++i) {
    if (cpu.gpr[i]._32 != ref_r->gpr[i]._32) {
      printf("%d号寄存器出错\n", i);
      printf("ref寄存器值为: %#x\n", ref_r->gpr[i]._32);
      // printf("a0寄存器值为: %#x\n", ref_r->gpr[10]._32);
      printf("ref的pc值: %#x\n", ref_r->pc);
      printf("cpu的pc值: %#x\n", cpu.pc);
      return false;
    }
  }
  return true;
}

void isa_difftest_attach() {
}

#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  for (int i = 0; i < 32; ++i) {
    if (i == 26 || i == 27)
      printf("%s           0x%x\n", reg_name(i, 0), gpr(i));
    else 
      printf("%s            0x%x\n", reg_name(i, 0), gpr(i));
  }
}

word_t isa_reg_str2val(const char *s, bool *success) {
  if (strcmp(s, "$pc") == 0) {
    return cpu.pc;
  }
  if (strcmp(s, "$0") == 0) {
    return gpr(0);
  }
  for (int i = 0; i < 32; ++i) {
    if (strcmp(s + 1, reg_name(i, 0)) == 0) {
      return gpr(i);
    }
  }
  fprintf(stderr, "%s is a non-existent register!\n", s);
  *success = false;
  return 0;
}

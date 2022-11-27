#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) f(addi) f(auipc) f(jal) f(add) \
f(beq) f(jalr) f(bne) f(sltiu) f(sub) f(sltu) f(xor) f(or) f(bge) f(slli) f(blt) f(sb) f(lbu) \
f(srai) f(mul) f(mulh) f(rem) f(andi) f(bltu) f(sh) f(sll) f(and) f(xori) f(srli) f(sra) f(srl) \
f(div) f(slt) f(lh) f(lhu) f(bgeu) f(divu) f(ori) f(lb) f(mulhu) f(remu) f(csrrw) f(ecall) f(csrrs) \
f(mret)

def_all_EXEC_ID();

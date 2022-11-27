def_EHelper(ecall) {
#ifdef CONFIG_ETRACE
  Log("%#x: 发生自陷!", cpu.pc);
#endif
  rtl_j(s, isa_raise_intr(0xb, cpu.pc));
}

def_EHelper(mret) {
#ifdef CONFIG_ETRACE
  Log("%#x: 从%d号异常返回!", cpu.csr.mepc, cpu.csr.mcause);
#endif
  // Log("%#x: 从%d号异常返回!", cpu.csr.mepc, cpu.csr.mcause);
  rtl_j(s, cpu.csr.mepc);
  // mstatus寄存器MIE域的值被更新为当前MPIE的值
  rtlreg_t MPIE = cpu.csr.mstatus & 0x80;
  if (MPIE == 0) {
    cpu.csr.mstatus &= ~0x8;
  }
  else {
    cpu.csr.mstatus |= 0x8;
  }
  // mstatus寄存器MPIE域的值则被更新为1
  cpu.csr.mstatus |= 0x80;
  // printf("sp: %#x\n", cpu.gpr[2]._32);
}
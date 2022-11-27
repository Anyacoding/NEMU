#include <isa.h>

#define IRQ_TIMER 0X80000007

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.csr.mepc = epc;
  cpu.csr.mcause = NO;
  rtlreg_t MIE = cpu.csr.mstatus & 0x8;
  if (MIE == 0) {
    cpu.csr.mstatus &= ~0x80;
  }
  else {
    cpu.csr.mstatus |= 0x80;
  }
  cpu.csr.mstatus &= ~0x8;
  return cpu.csr.mtvec;
}

word_t isa_query_intr() {
  rtlreg_t MIE = cpu.csr.mstatus & 0x8;
  // cpu处于开中断状态且收到时钟中断信号
  if (cpu.INTR == 1 && MIE != 0) {
    cpu.INTR = 0;
    return IRQ_TIMER;
  }
  return INTR_EMPTY;
}

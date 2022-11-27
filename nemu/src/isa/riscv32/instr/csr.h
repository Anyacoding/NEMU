def_EHelper(csrrw) {
  switch (id_src2->imm) {
    case 0x305:
      // rtl_li(s, ddest, cpu.csr.mtvec);
      // cpu.csr.mtvec = *dsrc1;
      rtl_li(s, s0, cpu.csr.mtvec);
      cpu.csr.mtvec = *dsrc1;
      rtl_mv(s, ddest, s0);
      break;
    case 0x341:
      // rtl_li(s, ddest, cpu.csr.mepc);
      // cpu.csr.mepc = *dsrc1;
      rtl_li(s, s0, cpu.csr.mepc);
      cpu.csr.mepc = *dsrc1;
      rtl_mv(s, ddest, s0);
      break;
    case 0x342:
      // rtl_li(s, ddest, cpu.csr.mcause);
      // cpu.csr.mcause = *dsrc1;
      rtl_li(s, s0, cpu.csr.mcause);
      cpu.csr.mcause = *dsrc1;
      rtl_mv(s, ddest, s0);
      break;
    case 0x300:
      // rtl_li(s, ddest, cpu.csr.mstatus);
      // cpu.csr.mstatus = *dsrc1;
      rtl_li(s, s0, cpu.csr.mstatus);
      cpu.csr.mstatus = *dsrc1;
      rtl_mv(s, ddest, s0);
      break;
    case 0x180:
      // rtl_li(s, ddest, cpu.csr.msatp);
      // cpu.csr.msatp = *dsrc1;
      rtl_li(s, s0, cpu.csr.msatp);
      cpu.csr.msatp = *dsrc1;
      rtl_mv(s, ddest, s0);
      break;
    case 0x340:
      // printf("csrrw before\n");
      // printf("mscratch: %#x\n", cpu.csr.mscratch);
      // printf("sp: %#x\n", cpu.gpr[2]._32);
      // printf("t3: %#x\n", cpu.gpr[28]._32);

      rtl_li(s, s0, cpu.csr.mscratch);
      cpu.csr.mscratch = *dsrc1;
      rtl_mv(s, ddest, s0);

      // printf("csrrw after\n");
      // printf("mscratch: %#x\n", cpu.csr.mscratch);
      // printf("sp: %#x\n", cpu.gpr[2]._32);
      // printf("t3: %#x\n\n", cpu.gpr[28]._32);
      break;
    default:
      panic("bad hit");
  }
}

def_EHelper(csrrs) {
  switch (id_src2->imm) {
    case 0x305:
      rtl_li(s, ddest, cpu.csr.mtvec);
      cpu.csr.mtvec |= *dsrc1;
      break;
    case 0x341:
      rtl_li(s, ddest, cpu.csr.mepc);
      cpu.csr.mepc |= *dsrc1;
      break;
    case 0x342:
      rtl_li(s, ddest, cpu.csr.mcause);
      cpu.csr.mcause |= *dsrc1;
      break;
    case 0x300:
      rtl_li(s, ddest, cpu.csr.mstatus);
      cpu.csr.mstatus |= *dsrc1;
      break;
    case 0x180:
      rtl_li(s, ddest, cpu.csr.msatp);
      cpu.csr.msatp |= *dsrc1;
      break;
    case 0x340:
      rtl_li(s, ddest, cpu.csr.mscratch);
      cpu.csr.mscratch |= *dsrc1;
      // printf("csrrs\n");
      // printf("mscratch: %#x\n", cpu.csr.mscratch);
      // printf("dest: %#x\n", *ddest);
      // printf("sp: %#x\n", cpu.gpr[2]._32);
      // printf("t3: %#x\n\n", cpu.gpr[28]._32);
      break;
    default:
      panic("bad hit");
  }
}
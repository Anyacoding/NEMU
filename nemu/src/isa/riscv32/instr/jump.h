def_EHelper(jal) {
  // TODO 已完成
  rtl_li(s, ddest, s->dnpc);
  rtl_j(s, id_src1->imm);
  //printf("%#x\n", s->dnpc);
}

def_EHelper(jalr) {
  // TODO 
  rtl_li(s, s0, s->dnpc);
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
  rtl_jr(s, ddest);
  rtl_li(s, ddest, *s0);
  //printf("%#x\n", s->dnpc);
}
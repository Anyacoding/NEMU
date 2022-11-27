def_EHelper(sltiu) {
  // rtl_li(s, s->dest.preg, id_src2->imm);
  if (*dsrc1 < (word_t)id_src2->imm) {
    rtl_li(s, ddest, 1);
  }
  else {
    rtl_li(s, ddest, 0);
  }
  // printf("%#x\n%d\n", s->dnpc, id_src2->imm);
}

def_EHelper(sltu) {
  // rtl_li(s, s->dest.preg, id_src2->imm);
  if (*dsrc1 < *dsrc2) {
    rtl_li(s, ddest, 1);
  }
  else {
    rtl_li(s, ddest, 0);
  }
  // printf("%#x\n%d\n", s->dnpc, id_src2->imm);
}

def_EHelper(slt) {
  // rtl_li(s, s->dest.preg, id_src2->imm);
  if ((sword_t)*dsrc1 < (sword_t)*dsrc2) {
    rtl_li(s, ddest, 1);
  }
  else {
    rtl_li(s, ddest, 0);
  }
  // printf("%#x\n%d\n", s->dnpc, id_src2->imm);
}
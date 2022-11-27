def_EHelper(beq) {
  if (*dsrc1 == *dsrc2) {
    rtl_j(s, id_dest->imm);
  }
  //panic("check beq");
  //printf("%#x\n", s->dnpc);
}

def_EHelper(bne) {
  if (*dsrc1 != *dsrc2) {
    rtl_j(s, id_dest->imm);
  }
  //panic("check beq");
  //printf("%#x\n", s->dnpc);
}

def_EHelper(bge) {
  if ((sword_t)*dsrc1 >= (sword_t)*dsrc2) {
    rtl_j(s, id_dest->imm);
  }
  //panic("check beq");
  //printf("%#x\n", s->dnpc);
}

def_EHelper(blt) {
  if ((sword_t)*dsrc1 < (sword_t)*dsrc2) {
    rtl_j(s, id_dest->imm);
  }
}

def_EHelper(bltu) {
  if (*dsrc1 < *dsrc2) {
    rtl_j(s, id_dest->imm);
  }
}

def_EHelper(bgeu) {
  if (*dsrc1 >= *dsrc2) {
    rtl_j(s, id_dest->imm);
  }
  //panic("check beq");
  //printf("%#x\n", s->dnpc);
}
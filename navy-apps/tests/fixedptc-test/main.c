#include <stdio.h>
#include "/home/anya/ics2021/navy-apps/libs/libfixedptc/include/fixedptc.h"

int main() {
  printf("floor(): %d\n", fixedpt_toint(fixedpt_floor(fixedpt_rconst(0.5))));
  printf("floor(): %d\n", fixedpt_toint(fixedpt_floor(fixedpt_rconst(-1.5))));
  printf("ceil(): %d\n", fixedpt_toint(fixedpt_ceil(fixedpt_rconst(0.3))));
  printf("ceil(): %d\n", fixedpt_toint(fixedpt_ceil(fixedpt_rconst(-2.3))));
  printf("abs(): %d\n", fixedpt_toint(fixedpt_abs(fixedpt_rconst(-2.3))));
  printf("muli(): %d\n", fixedpt_toint(fixedpt_muli(fixedpt_rconst(-2.5), 4)));
  printf("divi(): %d\n", fixedpt_toint(fixedpt_divi(fixedpt_rconst(-10), 2)));
  return 0;
}


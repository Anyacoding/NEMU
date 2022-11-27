#include <stdio.h>
#include "/home/anya/ics2021/navy-apps/libs/libndl/include/NDL.h"

void use_gettimeofday() {
  struct timeval now;
  gettimeofday(&now, NULL);
  uint64_t pre = now.tv_sec * 1000000 + now.tv_usec;
  while (1) {
    gettimeofday(&now, NULL);
    if (now.tv_sec * 1000000 + now.tv_usec - pre >= 500000) {
      printf("0.5s 过去了\n");
      pre = now.tv_sec * 1000000 + now.tv_usec;
    }
  }
}

void use_ndl() {
  uint64_t pre = NDL_GetTicks();
  uint64_t now = 0;
  while (1) {
    now = NDL_GetTicks();
    if (now - pre >= 500) {
      printf("0.5s 过去了\n");
      pre = now;
    }
  }
}

int main() {
  use_ndl();
  return 0;
}


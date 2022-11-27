#include <am.h>
#include <stdio.h>
#include <stdlib.h>
#include <klib-macros.h>

Area heap = {};

void __am_exit_platform(int code);


void putch(char ch) {
  putchar(ch);
}

void halt(int code) {
  const char *fmt = "Exit code = 40h\n";
  for (const char *p = fmt; *p; p++) {
    char ch = *p;
    if (ch == '0' || ch == '4') {
      ch = "0123456789abcdef"[(code >> (ch - '0')) & 0xf];
    }
    putch(ch);
  }
  exit(code);
  putstr("Should not reach here!\n");
  while (1);
}

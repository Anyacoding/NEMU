#include <common.h>
#include <unistd.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <sys/time.h>
#include <time.h>
#include "/home/anya/ics2021/navy-apps/libs/libos/src/syscall.h"
#include "/home/anya/ics2021/nemu/include/generated/autoconf.h"
#include <proc.h>
#include <stdio.h>

void naive_uload(PCB* pcb, const char* filename);
void context_uload(PCB *pcb, const char* pathname, char* argv[], char* const envp[]);
void switch_boot_pcb();
int mm_brk(uintptr_t brk);

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
char* fs_pathname(int fd);

void sys_yield(Context *c) {
  yield();
  c->GPRx = 0;
  c->mepc += 4;
#ifdef CONFIG_STRACE
  Log("调用SYS_yield, 系统调用号为%d, 返回值为0", c->GPR1);
#endif
}

void sys_exit(Context *c) {
#ifdef CONFIG_STRACE
  Log("调用SYS_exit, 系统调用号为%d, 返回值为%p", c->GPR1, c->GPRx);
#endif
  int status = c->GPR2;
  halt(status);
  char* argv[] = { "/bin/nterm", NULL };
  context_uload(current, "/bin/nterm", argv, NULL);
  switch_boot_pcb();
  yield();
}

void sys_write(Context *c) {
  int succes = 0;
  int fd = c->GPR2, count = c->GPR4;
  char* buf = (char*)c->GPR3;
  succes = fs_write(fd, (const void*)buf, count);
  c->mepc += 4;
  c->GPRx = succes;
#ifdef CONFIG_STRACE
  Log("调用SYS_write, 系统调用号为%d, 返回值为%d, mepc为%p", c->GPR1, c->GPRx, c->mepc);
#endif
}

void sys_brk(Context *c) {
  intptr_t program_break = c->GPR2;
  c->mepc += 4;
  c->GPRx = mm_brk(program_break);
#ifdef CONFIG_STRACE
  Log("调用SYS_brk, 系统调用号为%d, 返回值为%d, mepc为%p", c->GPR1, c->GPRx, c->mepc);
#endif
}

void sys_open(Context *c) {
  const char* pathname = (const char*)c->GPR2;
  int flags = c->GPR3, mode = c->GPR4;
  c->mepc += 4;
  c->GPRx = fs_open(pathname, flags, mode);
#ifdef CONFIG_STRACE
  Log("----------打开文件：%s ----------", pathname);
  Log("调用SYS_open, 系统调用号为%d, 返回值为%d, mepc为%p", c->GPR1, c->GPRx, c->mepc);
#endif
}

void sys_read(Context *c) {
  int fd = c->GPR2, count = c->GPR4;
  char* buf = (char*)c->GPR3;
  c->mepc += 4;
  c->GPRx = fs_read(fd, (void*)buf, count);
#ifdef CONFIG_STRACE
  Log("调用SYS_read, 系统调用号为%d, 返回值为%d, mepc为%p", c->GPR1, c->GPRx, c->mepc);
#endif
}

void sys_close(Context *c) {
  int fd = c->GPR2;
  c->mepc += 4;
  c->GPRx = fs_close(fd);
#ifdef CONFIG_STRACE
  Log("调用SYS_close, 系统调用号为%d, 返回值为%d, mepc为%p", c->GPR1, c->GPRx, c->mepc);
  Log("----------关闭文件：%s ----------", fs_pathname(fd));
#endif
}

void sys_lseek(Context *c) {
  int fd = c->GPR2, whence = c->GPR4;
  long offset = c->GPR3;
  c->mepc += 4;
  c->GPRx = fs_lseek(fd, offset, whence);
#ifdef CONFIG_STRACE
  Log("调用SYS_lseek, 系统调用号为%d, 返回值为%d, mepc为%p", c->GPR1, c->GPRx, c->mepc);
#endif
}

void sys_gettimeofday(Context *c) {
  struct timeval* tv = (struct timeval*)c->GPR2;
  // struct timezone* tz = (struct timezone*)c->GPR3;
  uint64_t us = io_read(AM_TIMER_UPTIME).us;
  tv->tv_sec = us / 1000000;
  tv->tv_usec = us - tv->tv_sec * 1000000;
  c->mepc += 4;
  c->GPRx = 0;
#ifdef CONFIG_STRACE
  Log("调用SYS_gettimeofday, 系统调用号为%d, 返回值为%d, mepc为%p", c->GPR1, c->GPRx, c->mepc);
#endif
}

void sys_execve(Context *c) {
#ifdef CONFIG_STRACE
  Log("调用SYS_execve, 系统调用号为%d", c->GPR1);
#endif
  
  const char* fname = (const char*)c->GPR2;
  char** argv = (char**)c->GPR3;
  char*const * envp = (char*const *)c->GPR4;
  // 先用fs_open来试探能不能打开
  int fd = fs_open(fname, 0, 0);
  // 打开失败就返回
  if (fd < 0) {
    c->mepc += 4;
    c->GPRx = -2; 
    return;
  }

  context_uload(current, fname, argv, envp);

  printf("execve filename: %s\n", fname);

  switch_boot_pcb();
  yield();
  //naive_uload(NULL, fname);
  c->GPRx = -1;
}


void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  switch (a[0]) {
    case SYS_yield:
      sys_yield(c);
      break;
    case SYS_exit:
      sys_exit(c);
      break;
    case SYS_write:
      sys_write(c);
      break;
    case SYS_brk:
      sys_brk(c);
      break;
    case SYS_open:
      sys_open(c);
      break;
    case SYS_read:
      sys_read(c);
      break;
    case SYS_close:
      sys_close(c);
      break;
    case SYS_lseek:
      sys_lseek(c);
      break;
    case SYS_gettimeofday:
      sys_gettimeofday(c);
      break;
    case SYS_execve:
      sys_execve(c);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

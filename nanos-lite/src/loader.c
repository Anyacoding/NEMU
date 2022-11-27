#include <proc.h>
#include <elf.h>
#include <fs.h>
#include <am.h>
#include "/home/anya/ics2021/abstract-machine/am/include/arch/riscv32-nemu.h"
#include <stdlib.h>
#include <string.h>
#include "/home/anya/ics2021/abstract-machine/klib/include/klib-macros.h"

#define PTE_V 0x01
#define PTE_R 0x02
#define PTE_W 0x04
#define PTE_X 0x08
#define PTE_U 0x10
#define PTE_A 0x40
#define PTE_D 0x80

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#define SIZE 0xfffff
// static uint8_t buf[SIZE];

size_t ramdisk_read(void* buf, size_t offset, size_t len);
size_t ramdisk_write(const void* buf, size_t offset, size_t len);

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
size_t fs_disk_offset(int fd);

void* new_page(size_t nr_page);
Context* ucontext(AddrSpace* as, Area kstack, void* sentry);

static void map_addr(AddrSpace* as, uintptr_t va, size_t size) {
  assert(va >= (uintptr_t)as->area.start);
  uintptr_t end_addr = ROUNDUP(va + size, PGSIZE);
  for (; va <= end_addr; va += PGSIZE) {
    void* pa = new_page(1);
    map(as, (void*)va, pa, 0);
  }
}

static uintptr_t loader(PCB *pcb, const char *filename) {
  printf("filename: %s\n", filename);
  // 解析ELF头
  Elf_Ehdr ehdr;
  int fd = fs_open(filename, 0, 0);

  // 打开失败，直接crash
  if (fd < 0) assert(0);
  fs_lseek(fd, 0, SEEK_SET);
  fs_read(fd, &ehdr, sizeof(Elf_Ehdr));

  // 检查魔数
  assert(*(uint32_t*)ehdr.e_ident == 0x464c457f);

  // 解析程序头表
  int count = ehdr.e_phnum;
  Elf_Phdr phdr[count];
  size_t disk_offset = fs_disk_offset(fd);
  ramdisk_read(&phdr, disk_offset + ehdr.e_phoff, count * sizeof(Elf_Phdr));

  for (int i = 0; i < count; ++i) {
    if (phdr[i].p_type == PT_LOAD) {
      // ramdisk_read(buf, disk_offset + phdr[i].p_offset, phdr[i].p_filesz);
      // bss置零
      // memset(buf + phdr[i].p_filesz, 0, phdr[i].p_memsz - phdr[i].p_filesz);

      // memcpy((void*)phdr[i].p_vaddr, buf, phdr[i].p_memsz);

      map_addr(&pcb->as, phdr[i].p_vaddr, phdr[i].p_memsz);
      fs_lseek(fd, phdr[i].p_offset, SEEK_SET);
      fs_read(fd, (void*)phdr[i].p_vaddr, phdr[i].p_filesz);
      memset((void*)phdr[i].p_vaddr + phdr[i].p_filesz, 0, phdr[i].p_memsz - phdr[i].p_filesz);
    }
  }
  fs_close(fd);
  // Log("Jump to entry = %p", ehdr.e_entry);
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

// 用户进程上下文装载
void context_uload(PCB *pcb, const char *pathname, char* argv[], char *const envp[]) {
  // 创建进程的地址空间
  if (pcb->as.ptr == NULL)
    protect(&(pcb->as));

  Area kstack = { .start = (void*)pcb->stack, .end = (void*)(pcb->stack +  STACK_SIZE)};
  void* entry = (void*)loader(pcb, pathname);
  // 打开失败，直接crash
  if (entry < 0) assert(0);

  pcb->cp = ucontext(&(pcb->as), kstack, entry);

  // 分配八页的内存作为用户栈
  void* va = pcb->as.area.end - 8 * PGSIZE;
  void* pa = new_page(8);
  int prot = 0;
  prot |= PTE_R | PTE_W | PTE_X;
  for (; va < (void*)ROUNDUP(pcb->as.area.end, PGSIZE); va += PGSIZE) {
    map(&(pcb->as), va, pa, prot);
    pa += PGSIZE;
  }
  
  char* sp = pcb->as.area.end;
  // printf("sp: %p\n", sp);
  // 解析argv[]，获得argc个数
  int argc = 0;
  while (argv[argc++] != NULL) ;
  --argc;
  // 计算出string area的大小
  int size = 0;
  for (int i = 0; i < argc; ++i) {
    // 这个+1是预留给\0的
    size = strlen(argv[i]) + 1;
    sp -= size;
    strcpy(sp, argv[i]);
    argv[i] = sp;
  }
  // 将argv[]压入ustack，注意一开始要塞一个NULL来作为argv和envp的分割线
  size = sizeof(char*);
  sp -= size;
  *(uint32_t*)sp = 0;
  for (int i = argc - 1; i >= 0; --i) {
    sp -= size;
    *(uint32_t*)sp = (uint32_t)argv[i];
  }
  // 将argc压入ustack
  size = sizeof(argc);
  sp -= size;
  *(int*)sp = argc;

  pcb->cp->GPRx = (uintptr_t)sp;

  pcb->cp->gpr[2] = (uintptr_t)sp;
  //printf("args: %p  argc: %d  argv[1]: %s\n", sp, *sp, *(uint32_t*)(sp + 8));
}
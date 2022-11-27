cmd_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/memory/vaddr.o := unused

source_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/memory/vaddr.o := src/memory/vaddr.c

deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/memory/vaddr.o := \
  /home/anya/ics2021/nemu/include/isa.h \
  /home/anya/ics2021/nemu/src/isa/riscv32/include/isa-def.h \
    $(wildcard include/config/itrace.h) \
  /home/anya/ics2021/nemu/include/common.h \
    $(wildcard include/config/target/am.h) \
    $(wildcard include/config/mbase.h) \
    $(wildcard include/config/msize.h) \
    $(wildcard include/config/isa64.h) \
  /home/anya/ics2021/nemu/include/macro.h \
  /home/anya/ics2021/nemu/include/debug.h \
  /home/anya/ics2021/nemu/include/utils.h \
    $(wildcard include/config/target/native/elf.h) \
  /home/anya/ics2021/nemu/include/memory/paddr.h \
    $(wildcard include/config/pc/reset/offset.h) \

/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/memory/vaddr.o: $(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/memory/vaddr.o)

$(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/memory/vaddr.o):

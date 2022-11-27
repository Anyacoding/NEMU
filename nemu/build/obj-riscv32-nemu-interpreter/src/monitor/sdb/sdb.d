cmd_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/monitor/sdb/sdb.o := unused

source_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/monitor/sdb/sdb.o := src/monitor/sdb/sdb.c

deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/monitor/sdb/sdb.o := \
    $(wildcard include/config/mbase.h) \
    $(wildcard include/config/msize.h) \
    $(wildcard include/config/device.h) \
  /home/anya/ics2021/nemu/include/isa.h \
  /home/anya/ics2021/nemu/src/isa/riscv32/include/isa-def.h \
    $(wildcard include/config/itrace.h) \
  /home/anya/ics2021/nemu/include/common.h \
    $(wildcard include/config/target/am.h) \
    $(wildcard include/config/isa64.h) \
  /home/anya/ics2021/nemu/include/macro.h \
  /home/anya/ics2021/nemu/include/debug.h \
  /home/anya/ics2021/nemu/include/utils.h \
    $(wildcard include/config/target/native/elf.h) \
  /home/anya/ics2021/nemu/include/cpu/cpu.h \
  src/monitor/sdb/sdb.h \
  /home/anya/ics2021/nemu/include/memory/paddr.h \
    $(wildcard include/config/pc/reset/offset.h) \
  src/monitor/sdb/watchpoint.h \

/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/monitor/sdb/sdb.o: $(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/monitor/sdb/sdb.o)

$(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/monitor/sdb/sdb.o):

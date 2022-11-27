cmd_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/io/port-io.o := unused

source_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/io/port-io.o := src/device/io/port-io.c

deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/io/port-io.o := \
  /home/anya/ics2021/nemu/include/device/map.h \
  /home/anya/ics2021/nemu/include/cpu/difftest.h \
    $(wildcard include/config/difftest.h) \
  /home/anya/ics2021/nemu/include/common.h \
    $(wildcard include/config/target/am.h) \
    $(wildcard include/config/mbase.h) \
    $(wildcard include/config/msize.h) \
    $(wildcard include/config/isa64.h) \
  /home/anya/ics2021/nemu/include/macro.h \
  /home/anya/ics2021/nemu/include/debug.h \
  /home/anya/ics2021/nemu/include/utils.h \
    $(wildcard include/config/target/native/elf.h) \
  /home/anya/ics2021/nemu/include/difftest-def.h \
    $(wildcard include/config/isa/x86.h) \
    $(wildcard include/config/isa/mips32.h) \
    $(wildcard include/config/isa/riscv32.h) \
    $(wildcard include/config/isa/riscv64.h) \

/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/io/port-io.o: $(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/io/port-io.o)

$(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/io/port-io.o):

cmd_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/audio.o := unused

source_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/audio.o := src/device/audio.c

deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/audio.o := \
    $(wildcard include/config/has/port/io.h) \
    $(wildcard include/config/audio/ctl/port.h) \
    $(wildcard include/config/audio/ctl/mmio.h) \
    $(wildcard include/config/sb/size.h) \
    $(wildcard include/config/sb/addr.h) \
  /home/anya/ics2021/nemu/include/common.h \
    $(wildcard include/config/target/am.h) \
    $(wildcard include/config/mbase.h) \
    $(wildcard include/config/msize.h) \
    $(wildcard include/config/isa64.h) \
  /home/anya/ics2021/nemu/include/macro.h \
  /home/anya/ics2021/nemu/include/debug.h \
  /home/anya/ics2021/nemu/include/utils.h \
    $(wildcard include/config/target/native/elf.h) \
  /home/anya/ics2021/nemu/include/device/map.h \
  /home/anya/ics2021/nemu/include/cpu/difftest.h \
    $(wildcard include/config/difftest.h) \
  /home/anya/ics2021/nemu/include/difftest-def.h \
    $(wildcard include/config/isa/x86.h) \
    $(wildcard include/config/isa/mips32.h) \
    $(wildcard include/config/isa/riscv32.h) \
    $(wildcard include/config/isa/riscv64.h) \

/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/audio.o: $(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/audio.o)

$(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/device/audio.o):

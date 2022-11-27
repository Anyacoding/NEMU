cmd_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/utils/timer.o := unused

source_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/utils/timer.o := src/utils/timer.c

deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/utils/timer.o := \
    $(wildcard include/config/timer/gettimeofday.h) \
    $(wildcard include/config/timer/clock/gettime.h) \
    $(wildcard include/config/target/am.h) \
  /home/anya/ics2021/nemu/include/common.h \
    $(wildcard include/config/mbase.h) \
    $(wildcard include/config/msize.h) \
    $(wildcard include/config/isa64.h) \
  /home/anya/ics2021/nemu/include/macro.h \
  /home/anya/ics2021/nemu/include/debug.h \
  /home/anya/ics2021/nemu/include/utils.h \
    $(wildcard include/config/target/native/elf.h) \

/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/utils/timer.o: $(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/utils/timer.o)

$(deps_/home/anya/ics2021/nemu/build/obj-riscv32-nemu-interpreter/src/utils/timer.o):

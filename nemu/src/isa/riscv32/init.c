#include <isa.h>
#include <memory/paddr.h>

// this is not consistent with uint8_t
// but it is ok since we do not access the array directly
static const uint32_t img [] = {
  0x800002b7,  // lui t0,0x80000
  0x0002a023,  // sw  zero,0(t0)
  0x0002a503,  // lw  a0,0(t0)
  0x0000006b,  // nemu_trap
};

static void restart() {
  /* Set the initial program counter. */
  cpu.pc = RESET_VECTOR;

  /* The zero register is always 0. */
  cpu.gpr[0]._32 = 0;

  // 将mstatus初始化为0x1800
  cpu.csr.mstatus = 0x1800;

  // 将msatp初始化为0，目的是为了判断是否开启分页
  cpu.csr.msatp = 0;

  // 将INTR引脚初始化为低电平
  cpu.INTR = 0;

  // 将ksp初始化为0
  cpu.csr.mscratch = 0;
}

void init_isa() {
  /* Load built-in image. */
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));

  /* Initialize this virtual computer system. */
  restart();
}

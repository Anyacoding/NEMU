#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

void __am_get_cur_as(Context* c);
void __am_switch(Context* c);
void get_mscratch();

Context* __am_irq_handle(Context *c) {
  __am_get_cur_as(c);
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 0xb: 
        ev.event = EVENT_YIELD; 
        // a7寄存器保存的是系统调用号
        if (c->GPR1 != -1) {
          ev.event = EVENT_SYSCALL;
        }
        break;
      case 0X80000007:
        ev.event = EVENT_IRQ_TIMER;
        break;
      default: ev.event = EVENT_ERROR; break;
    }
    // printf("in handle sp: %p\n", c->gpr[2]);
    // printf("in handle np before: %p\n", c->np);
    // printf("in handle pdir before: %p\n", c->pdir);
    c = user_handler(ev, c);
    // printf("in handle sp: %p\n", c->gpr[2]);
    // printf("in handle np after: %p\n", c->np);
    // printf("in handle pdir after: %p\n", c->pdir);

    assert(c != NULL);
  }
  __am_switch(c);
  // printf("退出__am_irq_handle\n\n");
  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  
  // 这里是用0号寄存器来存异常入口地址，因为是0号寄存器，后续会自动归0
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;
  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context* c = (Context*)kstack.end - sizeof(Context);
  c->mepc = (uintptr_t)entry;
  c->mstatus = 0x1880;
  c->mcause = 0xb;
  c->GPR2 = (uintptr_t)arg;
  c->pdir = NULL;
  c->np = 0;
  c->gpr[2] = (uintptr_t)c;
  printf("addr of hello: %p\n", c->mepc);
  return c;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}

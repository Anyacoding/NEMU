#include <common.h>
#include <am.h>
#include <proc.h>


void do_syscall(Context* c);

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD:  
#ifdef CONFIG_STRACE
      Log("自陷成功！");
#endif
      // Log("自陷调度！");
      c->mepc += 4;
      c = schedule(c);
      break;
    case EVENT_IRQ_TIMER:
      Log("时钟中断！");
      c = schedule(c);
      break;
    case EVENT_SYSCALL:
      do_syscall(c);
      break;
    default: panic("Unhandled event ID = %d", e.event);
  }
  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}

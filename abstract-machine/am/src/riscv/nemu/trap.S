#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)
#define MAP(c, f) c(f)

#if __riscv_xlen == 32
#define LOAD  lw
#define STORE sw
#define XLEN  4
#else
#define LOAD  ld
#define STORE sd
#define XLEN  8
#endif

#define REGS(f) \
      f( 1)       f( 3) f( 4) f( 5) f( 6) f( 7) f( 8) f( 9) \
f(10) f(11) f(12) f(13) f(14) f(15) f(16) f(17) f(18) f(19) \
f(20) f(21) f(22) f(23) f(24) f(25) f(26) f(27)       f(29) \ 
f(30) f(31) \

# not to save t3(28), it will be saved alone

#define PUSH(n) STORE concat(x, n), (n * XLEN)(sp);
#define POP(n)  LOAD  concat(x, n), (n * XLEN)(sp);

#define CONTEXT_SIZE   ((32 + 3 + 1 + 1) * XLEN)
#define OFFSET_SP      ( 2 * XLEN)
#define OFFSET_CAUSE   (32 * XLEN)
#define OFFSET_STATUS  (33 * XLEN)
#define OFFSET_EPC     (34 * XLEN)
#define OFFSET_NP      (36 * XLEN)
#define OFFSET_T3      (28 * XLEN) 
#define OFFSET_SP_RESTORE ( -35 * XLEN)

# whether to add mscratch to context???
# each pcb has it own kstack, technically
# we should do it, but in handout, it tend
# not to do it? 
# The answer is NO. In pcb, we already store
# the ksp(aka. cp), each time we invoke schedule()
# it will automatically store the ksp to pcb.cp.
# and when we reload this pcb to execute, schedule()
# will return the ksp to __am_asm_trap, so that it can 
# restore the context and set the mscratch as ksp value


# has bug!!!
.align 3
.globl __am_asm_trap
__am_asm_trap:
  csrrw sp, mscratch, sp   # (1) atomically exchange sp and mscratch
  bnez sp, save_context    # (2) take the branch if we trapped from user
  csrr sp, mscratch        # (3) if we trapped from kernel, restore the original sp

save_context: # now sp is pointing to kernel stack
  addi sp, sp, -CONTEXT_SIZE

  STORE t3, OFFSET_T3(sp)  # fitstly save t3, to get a free reg
  csrr t3, mscratch
  STORE t3, OFFSET_SP(sp)  # c->sp = $sp    $sp exchange into mscratch
  # now t3 and mscratch are free to use
  
  li t3, 0
  STORE t3, OFFSET_NP(sp)  # c->np = $t3 ------ KERNEL:0  USER:1
  csrr t3, mscratch
  addi sp, sp, CONTEXT_SIZE
  beq sp, t3, save_ksp
  addi sp, sp, -CONTEXT_SIZE
  li t3, 1
  STORE t3, OFFSET_NP(sp)
  addi sp, sp, CONTEXT_SIZE

save_ksp:
  addi sp, sp, -CONTEXT_SIZE

  csrw mscratch, x0        # ksp = 0  support re-entry of CTE

  MAP(REGS, PUSH)

  csrr t0, mcause
  csrr t1, mstatus
  csrr t2, mepc

  STORE t0, OFFSET_CAUSE(sp)
  STORE t1, OFFSET_STATUS(sp)
  STORE t2, OFFSET_EPC(sp)
# end save context

  # set mstatus.MPRV to pass difftest
  li a0, (1 << 17)
  or t1, t1, a0
  csrw mstatus, t1

  mv a0, sp  # parse the sp as argument
  jal __am_irq_handle
  mv sp, a0  # set the sp with return value of context pointer


  LOAD t3, OFFSET_NP(sp)  # load np to t3
  addi sp, sp, CONTEXT_SIZE
  beqz t3, restore_context
  csrw mscratch, sp       # restore the ksp to mscratch if the np == USER

restore_context:
  addi sp, sp, -CONTEXT_SIZE
  LOAD t1, OFFSET_STATUS(sp)
  LOAD t2, OFFSET_EPC(sp)
  csrw mstatus, t1
  csrw mepc, t2

  LOAD t3, OFFSET_T3(sp) # restore t3
  MAP(REGS, POP)         # In here a0's value is covered by context's a0

  LOAD sp, OFFSET_SP(sp) # restore the usp
# end restore context

  mret
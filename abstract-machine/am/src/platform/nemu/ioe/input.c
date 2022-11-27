#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000
#define CONFIG_I8042_DATA_MMIO 0xa0000060

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = 0;
  kbd->keycode = AM_KEY_NONE;
  uint32_t state = *(volatile uint32_t *)(CONFIG_I8042_DATA_MMIO);
  // decode出keycode
  kbd->keycode = state & ~KEYDOWN_MASK;
  if ((state & KEYDOWN_MASK) != 0) {
    kbd->keydown = true;
  }
}

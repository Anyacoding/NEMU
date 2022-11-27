#include <am.h>
#include <nemu.h>
#include <klib.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  // int i;
  // int w = inw(VGACTL_ADDR + 2);  // TODO: get the correct width
  // int h = inw(VGACTL_ADDR);      // TODO: get the correct height
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // for (i = 0; i < w * h; i ++) fb[i] = i;
  // outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 400, .height = 300,
    .vmemsz = 400 * 300 * 4
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
  uint32_t* fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t* pixels = ctl->pixels;
  // 屏幕大小
  int W = inw(VGACTL_ADDR + 2);
  int H = inw(VGACTL_ADDR);
  // 矩形块一次复制的大小
  int copy_size = sizeof(uint32_t) * ctl->w;
  // 防越界
  assert(ctl->x + ctl->w <= W);
  assert(ctl->y + ctl->h <= H);

  for (int i = 0; i < ctl->h && (i + ctl->y) < H; ++i) {
    memcpy(&fb[ctl->x + (ctl->y + i) * W], pixels, copy_size);
    pixels += ctl->w;
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}

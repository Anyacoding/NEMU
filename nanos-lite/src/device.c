#include <common.h>
#include <stddef.h>
#include <stdio.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  // yield();
  const char* str = buf;
  for (int i = 0; i < len; ++i) {
    putch(str[i]);
  }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  // yield();
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) return 0;

  if (ev.keydown == true) {
    sprintf(buf, "kd %s", keyname[ev.keycode]);
  }
  else {
    sprintf(buf, "ku %s", keyname[ev.keycode]);
  }
  //printf("%s\n", (char*)buf);
  return strlen(buf);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  int W = io_read(AM_GPU_CONFIG).width;
  int H = io_read(AM_GPU_CONFIG).height;
  sprintf(buf, "WIDTH:%d\nHEIGHT:%d\n", W, H);
  return strlen(buf);
}

static size_t rect_w, rect_h;
size_t rectinfo_write(const void *buf, size_t offset, size_t len) {
  rect_w = rect_h = 0;
  const char* temp = buf;
  int i = 0;
  for (; temp[i] >= '0' && temp[i] <= '9'; ++i) {
    rect_w = rect_w * 10 + temp[i] - '0';
  }
  for (++i; temp[i] >= '0' && temp[i] <= '9'; ++i) {
    rect_h = rect_h * 10 + temp[i] - '0';
  }
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  // yield();
  int W = io_read(AM_GPU_CONFIG).width;
  int x, y;
  int temp = offset / 4;
  y = temp / W;
  x = temp % W;
  io_write(AM_GPU_FBDRAW, x, y, (void*)buf, len / 4, 1, true);
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}

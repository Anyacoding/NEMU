#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static uint32_t boot_time;

void NDL_GetScreenSize(int *w, int *h) {
  // 解析出屏幕大小
  char buf[64] = {};
  int fd = open("/proc/dispinfo", 0, 0);
  read(fd, buf, sizeof(buf));
  sscanf(buf, "WIDTH:%d\nHEIGHT:%d\n", w, h);
}

// 以毫秒为单位返回系统时间
uint32_t NDL_GetTicks() {
  struct timeval now;
  gettimeofday(&now, NULL);
  return now.tv_sec * 1000 + now.tv_usec / 1000 - boot_time;
}

int NDL_PollEvent(char *buf, int len) {
  int fd = open("/dev/events", 0, 0);
  int ret = read(fd, buf, len);
  close(fd);
  return ret;
}

void NDL_OpenCanvas(int *w, int *h) {
  int W, H;
  NDL_GetScreenSize(&W, &H);
  if (*w == 0 && *h == 0) {
    *w = W;
    *h = H;
  }
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

static size_t center_off_x = 0;
static size_t center_off_y = 0;
static int is_recorded = 0;
static inline void init_offset(int x, int y, int w, int h) {
  if (x == 0 && y == 0 && is_recorded == 0) {
    int W, H;
    NDL_GetScreenSize(&W, &H);
    center_off_x = (W - w) / 2;
    center_off_y = (H - h) / 2;
    is_recorded = 1;
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int W, H;
  NDL_GetScreenSize(&W, &H);
  int fd = open("/dev/fb", 0, 0);
  init_offset(x, y, w, h);
  x += center_off_x;
  y += center_off_y;

  // 通过文件传送 w 和 h
  // char info[32];
  // sprintf(info, "%d\n%d", w, h);
  // int info_fd = open("W_H", 0, 0);
  // write(info_fd, info, 32);

  uint32_t color_buf[w];
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      color_buf[j] = pixels[i * w + j];
    }
    lseek(fd, (x + (y + i) * W) * 4, SEEK_SET);
    write(fd, color_buf, w * 1 * 4);
  }

}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  // 初始化时钟
  struct timeval now;
  gettimeofday(&now, NULL);
  boot_time = now.tv_sec * 1000 + now.tv_usec / 1000;
  return 0;
}

void NDL_Quit() {
}

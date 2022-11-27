#include <am.h>
#include <nemu.h>

static uint64_t boot_time = 0;

uint64_t read_time() {
  uint64_t hi = (uint64_t)inl(RTC_ADDR + 4) << 32;
  uint64_t lo = (uint64_t)inl(RTC_ADDR);
  return hi | lo;
}

void __am_timer_init() {
  boot_time = read_time();
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = read_time() - boot_time;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}

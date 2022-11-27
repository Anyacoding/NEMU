#include <fs.h>
#include <stdint.h>
#include <stddef.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

size_t ramdisk_read(void* buf, size_t offset, size_t len);
size_t ramdisk_write(const void* buf, size_t offset, size_t len);

size_t serial_write(const void* buf, size_t offset, size_t len);
size_t events_read(void* buf, size_t offset, size_t len);
size_t dispinfo_read(void* buf, size_t offset, size_t len);
size_t fb_write(const void* buf, size_t offset, size_t len);
size_t rectinfo_write(const void* buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
  bool overflow;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENT, FD_DISP, FD_W_H};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  [FD_FB]     = {"/dev/fb", 0, 0, invalid_read, fb_write},
  [FD_EVENT]  = {"/dev/events", 0, 0, events_read, invalid_write},
  [FD_DISP]   = {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
  [FD_W_H]    = {"W_H", 0, 0, invalid_read, rectinfo_write},
#include "files.h"
};

// 文件的数量
static int file_number = sizeof(file_table) / sizeof(Finfo) - 1;

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].size = 400 * 300 * 4;

  for (int i = 0; i < file_number; ++i) {
    file_table[i].open_offset = 0;
    file_table[i].overflow = false;
  }
}

int fs_open(const char *pathname, int flags, int mode) {
  for (int i = 0; i < file_number; ++i) {
    if (strcmp(pathname, file_table[i].name) == 0) {
      return i;
    }
  }
  return -2;
}

size_t fs_read(int fd, void *buf, size_t len) {
  if (file_table[fd].write != NULL) {
    return file_table[fd].read(buf, file_table[fd].open_offset, len);
  }
  else {
    size_t temp = file_table[fd].size - file_table[fd].open_offset;
    if (len > temp) len = temp;
    ramdisk_read(buf, file_table[fd].open_offset + file_table[fd].disk_offset, len);
  }
  file_table[fd].open_offset += len;
  return len;
}

size_t fs_write(int fd, const void *buf, size_t len) {
  if (file_table[fd].write != NULL) {
    file_table[fd].write(buf, file_table[fd].open_offset, len);
  }
  else {
    size_t temp = file_table[fd].size - file_table[fd].open_offset;
    if (len > temp) len = temp;
    ramdisk_write(buf, file_table[fd].open_offset + file_table[fd].disk_offset, len);
  }
  file_table[fd].open_offset += len;
  return len;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  size_t ret = 0;
  switch (whence) {
    case SEEK_SET:
      ret = offset;
      file_table[fd].open_offset = ret;
      break;
    case SEEK_CUR:
      ret = file_table[fd].open_offset + offset;
      file_table[fd].open_offset = ret;
      break;
    case SEEK_END:
      ret = file_table[fd].size + offset;
      file_table[fd].open_offset = ret;
      break;
    default:
      ret = -1;
  }

  if (!(ret >= 0 && ret < file_table[fd].size)) {
    file_table[fd].overflow = true;
  }

  return ret;
}

int fs_close(int fd) {
  file_table[fd].open_offset = 0;
  return 0;
}

size_t fs_disk_offset(int fd) {
  return file_table[fd].disk_offset;
}

char* fs_pathname(int fd) {
  return file_table[fd].name;
}
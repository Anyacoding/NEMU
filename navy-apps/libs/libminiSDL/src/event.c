#include <NDL.h>
#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

static char keyState[83] = {};

int SDL_PushEvent(SDL_Event *event) {
  printf("没有实现!\n");
  assert(0);
  return 0;
}

int SDL_PollEvent(SDL_Event *event) {
  static char buf[64];
  static char type[8];
  static char name[16];
  memset(buf, 0, sizeof(buf));
  // 如果没有事件直接返回
  if (NDL_PollEvent(buf, sizeof(buf)) == 0) return 0;

  memset(type, 0, sizeof(type));
  memset(name, 0, sizeof(name));
  sscanf(buf, "%s %s", type, name);
  //printf("%s %s", type, name);

  int n = sizeof(keyname) / sizeof(char*);
  for (int i = 0; i < n; ++i) {
    if (strcmp(name, keyname[i]) == 0) {
      event->key.keysym.sym = i;
      break;
    }
  }

  if (strcmp(type, "kd") == 0) {
    event->type = SDL_KEYDOWN;
    keyState[event->key.keysym.sym] = 1;
  }
  else {
    event->type = SDL_KEYUP;
    keyState[event->key.keysym.sym] = 0;
  }
  return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
  while (SDL_PollEvent(event) == 0);
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  printf("没有实现!\n");
  assert(0);
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  if (numkeys) *numkeys = sizeof(keyState);
  return keyState;
}

#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);
void clear_screen();

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
  char pathname[128] = {};
  char buf[32] = {};
  int i = 0;
  int j = 0;
  while (cmd[i] != ' ' && cmd[i] != '\n') {
    pathname[i] = cmd[i];
    ++i;
  }
  ++i;
  while (cmd[i] != '\n') {
    buf[j] = cmd[i];
    ++i;
    ++j;
  }
  char* argv[] = { pathname, buf, NULL };
  clear_screen();
  execvp(pathname, argv);
  assert(0);
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}

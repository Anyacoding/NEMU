#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <memory/paddr.h>
#include "watchpoint.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args) {
  uint64_t n = 1;
  if (args != NULL) { n = atoi(args); }
  cpu_exec(n);
  return 0;
}

static int cmd_info(char *args) {
  if (args == NULL) { return 0; }
  if (args[0] == 'r') {
    isa_reg_display();
  }
  else if (args[0] == 'w') {
    print_wp();
  }
  return 0;
}

static int cmd_x(char *args) {
  char *str_end = args + strlen(args);
  char *cmd = strtok(args, " ");
  if (cmd == NULL) { return 0; }
  
  int n = 0;
  sscanf(cmd, "%d", &n);
  
  char* cmd2 = cmd + strlen(cmd) + 1;
  if (cmd2 >= str_end) { printf("y\n"); return 0; }
  
  paddr_t addr = 0;
  sscanf(cmd2, "%x", &addr);

  unsigned line = 0;
  printf("0x%x:", addr);
  for (int i = 0; i < n; ++i) {
    if (addr + i * 4 >= CONFIG_MBASE + CONFIG_MSIZE || addr < CONFIG_MBASE) {
      printf("     Cannot access memory at address %#x\n", addr + i * 4);
      return 0;
    }

    word_t temp = paddr_read(addr + i * 4, 4);
    printf("     %#x", temp);
    if ((i + 1) % 4 == 0) {
      ++line;
      printf("\n0x%x:", addr + 16 * line);
    }

  }
  printf("\n");
  return 0;
}

static int cmd_p(char *args) {
  bool success = true;
  expr(args, &success);
  return success;
}

static int cmd_w(char *args) {
  bool success = true;
  word_t ret = expr(args, &success);

  if (success == false) {
    printf("创建监视点失败\n");
    return success;
  }
  WP* watch = new_wp();
  strcpy(watch->expr, args);
  watch->old = watch->new = ret;
  printf("创建监视点%d成功\n", watch->NO);
  return success;
}

static int cmd_d(char *args) {
  WP* del = find_wp(atoi(args));

  if (del != NULL) {
    free_wp(del);
    printf("删除监视点%s成功\n", args);
  }
  else {
    printf("删除监视点%s失败\n", args);
  }

  return 0;
}

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", " - Display informations about all supported commands", cmd_help },
  { "c", "    - Continue the execution of the program", cmd_c },
  /* TODO: Add more commands */
  { "si", "   - Let the program step through N instructions and then pause,\n       when N is not given, the default is 1\n       (eg: si 10", cmd_si },
  { "info", " - Print register state\n       (eg: info r\n       Prints monitoring point information\n       (eg: info w", cmd_info },
  { "x", "    - Evaluate the expression EXPR and use the result as the\n       starting memory Address, output N consecutive 4-bytes in hexadecimal format\n       (eg: x 10 $esp", cmd_x },
  { "p", "    - Evaluate the expression EXPR\n       (eg: p $eax + 1", cmd_p },
  { "w", "    - Suspends program execution when the value of the expression EXPR changes\n       (eg: w *0x2000", cmd_w },
  { "d", "    - Delete the monitor number N\n       (eg: d 2", cmd_d },
  /* TODO: End */
  { "q", "    - Exit NEMU", cmd_q },

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s%s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}

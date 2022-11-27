#include <isa.h>
#include <memory/paddr.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

#define ll long long

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NEQ,

  /* TODO: Add more token types */
  TK_DIG = 0, TK_DEREF = 1,
  TK_HEX = 2, TK_REG = 3,
  TK_NEG = 4, TK_AND = 5,
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"\\-", '-'},                     // sub
  {"\\*", '*'},                     // multi
  {"/", '/'},                       // div
  {"\\(", '('},                     // bracket-left
  {"\\)", ')'},                     // bracket-right
  {"0[Xx][0-9a-fA-F]+", TK_HEX},    // hex
  {"[0-9]+", TK_DIG},               // dig
  {"\\$[0-9a-z]+", TK_REG},         // reg
  {"==", TK_EQ},                    // equal
  {"!=", TK_NEQ},                   // not equal
  {"&&", TK_AND},                   // &&

};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case '+':
            tokens[nr_token++].type = '+';
            break;
          case '-':
            tokens[nr_token++].type = '-';
            break;
          case '*':
            tokens[nr_token++].type = '*';
            break;
          case '/':
            tokens[nr_token++].type = '/';
            break;
          case '(':
            tokens[nr_token++].type = '(';
            break;
          case ')':
            tokens[nr_token++].type = ')';
            break;
          case TK_DIG: 
            if (substr_len >= 32) {
              fprintf(stderr, "Number out of buffer!\n");
              return false;
            }
            tokens[nr_token].type = TK_DIG;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            // 因为strncpy不会在字符串末尾自动补'\0'，非常之坑爹，所以我们手动补0
            tokens[nr_token++].str[substr_len] = '\0';
            break;
          case TK_HEX: 
            if (substr_len >= 32) {
              fprintf(stderr, "Number out of buffer!\n");
              return false;
            }
            tokens[nr_token].type = TK_HEX;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token++].str[substr_len] = '\0';
            break;
          case TK_REG:
            if (substr_len >= 32) {
              fprintf(stderr, "Name of register out of buffer!\n");
              return false;
            }
            tokens[nr_token].type = TK_REG;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token++].str[substr_len] = '\0';
            break;
          case TK_EQ:
            tokens[nr_token++].type = TK_EQ;
            break;
          case TK_NEQ:
            tokens[nr_token++].type = TK_NEQ;
            break;
          case TK_AND:
            tokens[nr_token++].type = TK_AND;
            break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

/* TODO: Insert codes to evaluate the expression. */

// 一个简易的stack库
typedef struct stack {
  ll arr[32];
  int top;
} my_stack;

bool empty(my_stack* s) {
  return s->top >= 0;
}

void push(my_stack* s, ll val) {
  s->arr[++(s->top)] = val;
}

void pop(my_stack* s) {
  if (empty(s)) {
    --(s->top);
  }
}

ll top(my_stack* s) {
  return s->arr[s->top];
}

ll size(my_stack* s) {
  return s->top + 1;
}

// 符号的优先级
int level(ll c) {
  switch (c) {
    case TK_AND:
      return 2;
    case TK_EQ:
    case TK_NEQ:
      return 4;
    case '-':
    case '+':
      return 5;
    case '*':
    case '/':
      return 6;
    case TK_NEG:
    case TK_DEREF:
      return 7;
    default:
      break;
  }
  return 0;
}

void eval(my_stack* num, my_stack* op) {
  ll a = 0;
  ll b = 0;
  ll val = 0;

  if (top(op) == TK_NEG || top(op) == TK_DEREF) {
    a = top(num); pop(num);
  }
  else {
    a = top(num); pop(num);
    b = top(num); pop(num);
  }
  
  switch (top(op)) {
    case '+': 
      push(num, b + a); break;
    case '-': 
      push(num, b - a); break;
    case '*': 
      push(num, b * a); break;
    case '/': 
      push(num, b / a); break;
    case TK_DEREF:
      val = paddr_read((paddr_t)a, 4);
      push(num, val); break;
    case TK_NEG:
      push(num, -1 * a); break;
    case TK_EQ:
      push(num, a == b); break;
    case TK_NEQ:
      push(num, a != b); break;
    case TK_AND:
      push(num, a && b); break;
    case '(':
      panic("The right parenthesis does not match!\n");
    default:
      panic("Illegal symbol!\n");
    }
  pop(op);
  return;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  my_stack num = { {0}, 0 };
  my_stack op = { {}, -1 };
  int i = 0;
  
  for (i = 0; i < nr_token; ++i) {
    if (tokens[i].type == '(') {
      push(&op, tokens[i].type);
    }
    else if (tokens[i].type == ')') {
      bool match = false;
      while (empty(&op) && size(&num) >= 2) {
        if (top(&op) != '(') {
          eval(&num, &op);
        }
        else {
          pop(&op);
          match = true;
          break;
        }
      }
      if (!match) { panic("The left parenthesis does not match!\n"); }
    }
    else if (tokens[i].type == TK_DIG) {
      push(&num, atoll(tokens[i].str));
    }
    else if (tokens[i].type == TK_HEX) {
      ll val = 0;
      sscanf(tokens[i].str, "%llx", &val);
      push(&num, val);
      tokens[i].type = TK_DIG;
    }
    else if (tokens[i].type == TK_REG) {
      bool success = true;
      ll val = (ll)isa_reg_str2val(tokens[i].str, &success);
      push(&num, val);
      tokens[i].type = TK_DIG;
      if (!success) { break; }
    }
    else {
      // 拓展解引用功能
      if (tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type != TK_DIG && tokens[i - 1].type != ')'))) {
        if (i == 0) {
          tokens[i].type = TK_DEREF;
          //puts("TK_DEREF");
        }
        else if (tokens[i + 1].type == TK_DIG || tokens[i + 1].type == TK_HEX || tokens[i + 1].type == TK_REG) {
          tokens[i].type = TK_DEREF;
          //puts("TK_DEREF");
        }
      }
      // 将 (- 替换为 (0- , (+ 替换为 (0+ , 这样当出现 (-100) 也能正确求值
      // else if (i > 0 && tokens[i - 1].type == '(') {
      //   push(&num, 0);
      //   puts("shit");
      // }
      else if (tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type != TK_DIG && tokens[i - 1].type != ')'))) {
        if (i == 0) {
          tokens[i].type = TK_NEG;
          //puts("TK_NEG");
        }
        else if (tokens[i + 1].type == TK_DIG || tokens[i + 1].type == TK_HEX || tokens[i + 1].type == TK_REG) {
          tokens[i].type = TK_NEG;
          //puts("TK_NEG");
        }
        else if (tokens[i + 1].type == '*') {
          tokens[i].type = TK_NEG;
          //puts("TK_NEG");
        }
      }

      while (empty(&op) && top(&op) != '(' && level(top(&op)) >= level(tokens[i].type) && size(&num) >= 2) {
        eval(&num, &op);
      }

      push(&op, tokens[i].type);
      
    }
  }

  while (empty(&op)) {
    eval(&num, &op);
  }

  //if (empty(&num)) { printf("表达式结果：%#llx\n", top(&num)); }

  return top(&num);

}

ll val = 0xffffff10;

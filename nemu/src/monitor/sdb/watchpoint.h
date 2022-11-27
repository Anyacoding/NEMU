#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

  // 表达式旧值
  word_t old; 
  // 表达式新值
  word_t new;    
  // 表达式  
  char expr[1024];         
} WP;

WP* new_wp();

void free_wp(WP* wp);

bool check_wp();

WP* find_wp(int NO);

void print_wp();

#endif
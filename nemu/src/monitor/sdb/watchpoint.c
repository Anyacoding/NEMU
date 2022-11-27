#include "sdb.h"
#include "watchpoint.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp() {
  // 断言，保证空闲列表中还有资源，若没有则直接abort
  assert(free_ != NULL);
  // 从空闲列表中获取资源
  WP* temp = free_;
  free_ = free_->next;
  // 头插法建立链表
  temp->next = head;
  head = temp;
  return temp;
}

void free_wp(WP* wp) {
  WP* cur = head;
  WP* pre = cur;
  // 寻找待删除节点
  while (wp != cur) {
    assert(cur != NULL);
    pre = cur;
    cur = cur->next;
  }

  // 如果wp是头结点的话,特殊处理
  if (wp == head) {
    head = head->next;
  }

  // 将cur从使用列表中删除
  pre->next = cur->next;
  // 将cur加入到空闲列表中
  cur->next = free_;
  free_ = cur;
}

bool check_wp() {
  WP* temp = head;
  bool success = true;
  // if (temp == NULL) {
  //     printf("没有监视点需要检查\n");
  //     return success;
  // }
  while (temp) {
    word_t now = expr(temp->expr, &success);
    //printf("%s     %#x\n", temp->expr, temp->new);
    if (temp->new != now) {
      printf("用户上一条指令触发监视点%d!\n", temp->NO);
      temp->old = temp->new;
      temp->new = now;
      success = false;
      return success;
    }
    temp = temp->next;
  }
  return success;
}

WP* find_wp(int NO) {
  WP* temp = head;
  while (temp) {
    if (temp->NO == NO) {
      return temp;
    }
    temp = temp->next;
  }
  return NULL;
}

void print_wp() {
  WP* temp = head;
  while (temp) {
    printf("Num     Type           Disp Enb Address            What\n");
    printf("%d       watchpoint     keep y                      %s\n", temp->NO, temp->expr);
    temp = temp->next;
  }
}

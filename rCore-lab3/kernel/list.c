// Most of this file is copy from xv6-riscv-fall19 lab alloc(https://github.com/mit-pdos/xv6-riscv-fall19/tree/alloc)
#include "kernel.h"
// 定义了链表结构以及相关的查找、插入等基本操作，这是建立基于链表方法的物理内存管理（以及其他内核功能）的基础
//其他有类似双向链表需求的内核功能模块可直接使用 list.h 中定义的函数
// double-linked, circular list. double-linked makes remove
// fast. circular simplifies code, because don't have to check for
// empty list in insert and remove.

//链表初始化
void
lst_init(struct list *lst)
{
  lst->next = lst;
  lst->prev = lst;
}
//清空
int
lst_empty(struct list *lst) {
  return lst->next == lst;
}
//移除
void
lst_remove(struct list *e) {
  e->prev->next = e->next;
  e->next->prev = e->prev;
}
//出栈
void*
lst_pop(struct list *lst) {
  if(lst->next == lst)
    panic("lst_pop");
  struct list *p = lst->next;
  lst_remove(p);
  return (void *)p;
}
//入栈
void
lst_push(struct list *lst, void *p)
{
  struct list *e = (struct list *) p;
  e->next = lst->next;
  e->prev = lst;
  lst->next->prev = p;
  lst->next = e;
}
//打印
void
lst_print(struct list *lst)
{
  for (struct list *p = lst->next; p != lst; p = p->next) {
    printf("%p", p);
  }
  printf("\n");
}
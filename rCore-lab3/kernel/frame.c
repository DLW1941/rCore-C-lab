#include "kernel.h"
//空闲内存的起始物理页号、结束物理页号
PhysPageNum current, end;
struct rnode {
    struct list lnode;
    usize ppn;
};
//recycled 以 后入先出 的方式保存被回收的物理页号
struct list *recycled;
usize frame_num;
//物理页帧初始化：CEIL向上取整，FLOOR向下取整
void frame_init() {
    extern char ekernel;
    current = CEIL((usize)&ekernel);//物理内存左端点 ekernel
    end = FLOOR(MEMORY_END);//物理内存右端点 MEMORY_END
    recycled = bd_malloc(sizeof(struct list));
    lst_init(recycled);
    frame_num = end - current;
}
//分配页号：若 回收链表 不空则返回 回收链表 栈顶页号
//若当前物理页号current=结束页号end，说明物理内存已满，报错
//否则，返回新页ppn=current++
PhysPageNum frame_alloc() {
    int ret; PhysPageNum ppn;
    if (! lst_empty(recycled)) {
        struct rnode *x = lst_pop(recycled);
        ppn = x->ppn; bd_free(x);
    } else {
        if (current == end) panic("frame_alloc: no free physical page");
        else ppn = current++;
    }
    memset((void *)PPN2PA(ppn), 0, PAGE_SIZE);
    frame_num--;
    print_frame_num();
    return ppn;
}
//释放页号:先检查回收页面的合法性，然后将其压入 recycled 栈中
//回收页面合法有两个条件：
//该页面之前一定被分配出去过，因此它的物理页号一定< current 
//该页面没有正处在回收状态，即它的物理页号不能在栈 recycled 中找到
void frame_dealloc(PhysPageNum ppn) {
    if (ppn >= current) goto fail;
    for (struct list *p = recycled->next; p != recycled; p = p->next) {
        if (((struct rnode *)p)->ppn == ppn) goto fail;
    }
    struct rnode *x = bd_malloc(sizeof(struct rnode));
    x->ppn = ppn; lst_push(recycled, x);
    frame_num++;
    print_frame_num();
    return;
fail: panic("frame_dealloc failed!");
}
//打印页帧号
void print_frame_num() {
    printf("frame_num:%d\n", frame_num);
}
#include "kernel.h"
#include "queue.h"
//创建进程的起始物理页号、结束物理页号
PhysPageNum fcurrent, fend;
typedef struct flist {
    PhysPageNum ppn; LIST_ENTRY(flist) entries;
} flist;
LIST_HEAD(flist_head, flist);
struct flist_head frecycled;
usize frame_num;
//物理页帧初始化：CEIL向上取整，FLOOR向下取整
void frame_init() {
    extern char ekernel;
    fcurrent = CEIL((usize)&ekernel);
    fend = FLOOR(MEMORY_END);
    LIST_INIT(&frecycled);
    frame_num = fend - fcurrent;
}
//分配页号：若 回收链表 不空则返回 回收链表 栈顶页号
//若进程物理页号fcurrent=结束页号fend，说明物理内存已满，报错
//否则，返回新页ppn=fcurrent++
PhysPageNum frame_alloc() {
    PhysPageNum ppn;
    if (! LIST_EMPTY(&frecycled)) {
        flist *x = LIST_FIRST(&frecycled);
        LIST_REMOVE(x, entries);
        ppn = x->ppn; bd_free(x);
    } else {
        if (fcurrent == fend) panic("frame_alloc: no free physical page");
        else ppn = fcurrent++;
    }
    memset((void *)PPN2PA(ppn), 0, PAGE_SIZE);
    // printf("frame_alloc:remain%d\n", --frame_num);
    return ppn;
}
//释放页号:先检查回收页面的合法性，然后将其压入 frecycled 栈中
//回收页面合法有两个条件：
//该页面之前一定被分配出去过，因此它的物理页号一定< fcurrent 
//该页面没有正处在回收状态，即它的物理页号不能在栈 frecycled 中找到
void frame_dealloc(PhysPageNum ppn) {
    if (ppn >= fcurrent) goto fail;
    flist *x;
    LIST_FOREACH(x, &frecycled, entries) {
        if (x->ppn == ppn) goto fail;
    }
    x = bd_malloc(sizeof(flist));
    x->ppn = ppn; LIST_INSERT_HEAD(&frecycled, x, entries);
    // printf("frame_dealloc:remain%d\n", ++frame_num);
    return;
fail: panic("frame_dealloc failed!");
}
//打印页帧号
void print_frame_num() {
    printf("frame_num:%d\n", frame_num);
}
#include "kernel.h"
#define MAX_APP_NUM 10
#define APP_BASE_ADDRESS 0x80400000       //应用程序的起始物理地址
#define USER_STACK_SIZE (4096 * 2)                //用户栈大小为8KB
#define KERNEL_STACK_SIZE (4096 * 2)          //内核栈大小为8KB

usize num_app;
usize current_app = 0;
usize app_start[MAX_APP_NUM + 1];

void __alltraps();
//
void load_all() {
    extern usize _num_app;
    usize *num_app_ptr = &_num_app;
    num_app = _num_app;
    for (usize i = 1; i <= num_app + 1; i++)
        app_start[i - 1] = num_app_ptr[i];
    asm volatile ("csrw stvec, %0"::"r"(__alltraps));
    run_next_app();
}
//先判断批处理的应用程序数；若未超出则清空内存，准备装载应用程序
void load_app(usize app_id) {
    if (app_id >= num_app) exit_all();
    asm volatile("fence.i");
    usize base = app_start[app_id];
    for (usize i = app_start[app_id]; i < app_start[app_id + 1]; i++) {
        *(char *)(APP_BASE_ADDRESS + i - base) = *(char *)i;
    }
}
char KERNEL_STACK[KERNEL_STACK_SIZE], USER_STACK[USER_STACK_SIZE];
const char *KERNEL_TOP = KERNEL_STACK + KERNEL_STACK_SIZE;
const char *USER_TOP = USER_STACK + USER_STACK_SIZE;
void __restore(usize);

TrapContext *app_init_context(usize entry, usize sp, TrapContext *cx) {
//Trap上下文（即数据结构 TrapContext ）
//类似前面提到的函数调用上下文，即在 Trap 发生时需要保存的物理资源内容
//并将其一起放在一个名为 TrapContext 的类型中
    usize sstatus; asm volatile("csrr %0, sstatus":"=r"(sstatus));
// C语言调库麻烦，这里就硬编码了，sstatus的第8位（从0开始）指定程序所处的态，0为用户态，1为内核态
    sstatus &= ~(1L << 8);
    for (int i = 0; i < 32; i++) cx->x[i] = 0;
    cx->sepc = entry; cx->sstatus = sstatus;
    cx->x[2] = sp; return cx;
}
//批处理操作系统的核心操作: 加载并运行下一个应用程序
void run_next_app() {
    load_app(current_app); current_app++;
    __restore((usize)app_init_context(
                APP_BASE_ADDRESS,
                (usize)USER_TOP,
                (TrapContext *)(KERNEL_TOP - sizeof(TrapContext))
                ));
}

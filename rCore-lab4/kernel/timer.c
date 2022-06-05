#include "kernel.h"

#ifdef K210
#define CLOCK_FREQ (403000000 / 62)
#else
#define CLOCK_FREQ 12500000
#endif

#define TICKS_PER_SEC 100
#define MSEC_PER_SEC 1000

//读取时间寄存器mtime
usize get_time() {
    usize timer; asm volatile("rdtime %0":"=r"(timer)); return timer;
}
//设置mtimecmp的触发值
void set_next_trigger() {
    set_timer(get_time() + CLOCK_FREQ / TICKS_PER_SEC);
}
//计时：以微秒为单位返回当前计数器的值，统计一个应用的运行时长
usize get_time_ms() {
    return get_time() / (CLOCK_FREQ / MSEC_PER_SEC);
}
#include "kernel.h"

#define CLOCK_FREQ (403000000 / 62)         //6.5MHz
#define CLOCK_FREQ 12500000                 //12.5MHz

#define TICKS_PER_SEC 100
#define MSEC_PER_SEC 1000
// 读取时间寄存器mtime
usize get_time() {
    usize timer; 
    asm volatile("rdtime %0":"=r"(timer)); 
    return timer;
}
//set_next_trigger 函数对 set_timer 进行封装
//它首先读取当前 mtime 的值，然后计算出10ms之内计数器的增量，再将 mtimecmp 设置为二者的和
//这样，10ms 之后一个 S 特权级时钟中断就会被触发
void set_next_trigger() {
    set_timer(get_time() + CLOCK_FREQ / TICKS_PER_SEC);
}
//计时：以微秒为单位返回当前计数器的值，统计一个应用的运行时长
usize get_time_ms() {
    return get_time() / (CLOCK_FREQ / MSEC_PER_SEC);
}



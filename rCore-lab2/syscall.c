#include "kernel.h"
//写
isize sys_write(usize fd, char *buffer, usize len) {
    switch (fd) {
        case FD_STDOUT:
            printf(buffer);
            return (isize)len;
    }
}
//lab2中新增系统调用 sys_yield ：程序主动暂停
isize sys_yield() {
    suspend_current_and_run_next();
    return 0;
}
//程序主动退出
isize sys_exit(int xstate) {
    printf("Application has done and exited. %d \n", xstate);
    exit_current_and_run_next();
}
//lab2中新增系统调用 sys_get_time ：定时器相关
isize sys_get_time() {
    return (isize)get_time_ms();
}

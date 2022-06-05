#include "kernel.h"

/// 功能：将内存中缓冲区中的数据写入文件。
/// 参数：`fd` 表示待写入文件的文件描述符；
///       `buffer` 表示内存中缓冲区的起始地址；
///       `len` 表示内存中缓冲区的长度。
/// 返回值：返回成功写入的长度。
isize sys_write(usize fd, char *buffer, usize len) {
    switch (fd) {
        case FD_STDOUT:
            printf(buffer);
            return (int)len;
    }
}

/// 功能：退出应用程序并将返回值告知批处理系统。
/// 参数：`xstate` 表示应用程序的返回值。
/// 返回值：该系统调用不应该返回。
isize sys_exit(int xstate) {
    printf("Application exited with code %d \n", xstate);
    run_next_app();
}

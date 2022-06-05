#include "user.h"
//由于支持的系统调用有限，目前只有往标准输出打印一个Hello word！然后返回退出的功能
int main() {
    write(FD_STDOUT, "Hello World!\n");
    return 0;
}

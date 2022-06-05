typedef unsigned long long usize;
typedef long long isize;

//需要用到的SBI函数及其编号对应
const usize SBI_PUTCHAR = 1;
const usize SBI_SHUTDOWN = 8;

isize sbi_call(usize id, usize a0, usize a1, usize a2) {//arg0 ~ arg2 表示传递给 RustSBI 的 3 个参数
    isize ret;
    asm volatile (
            "mv x10, %1\n"
            "mv x11, %2\n"
            "mv x12, %3\n"
            "mv x17, %4\n"//mv操作把参数的数值放到寄存器里
            "ecall\n"//参数放好之后，通过ecall, 交给RustSBI来执行
            "mv %0, x10\n"//SBI按照riscv的calling convention,把返回值放到x10寄存器里
            //通过内联汇编把返回值拿到我们的变量里
            :"=r"(ret)
            :"r"(a0), "r"(a1), "r"(a2), "r"(id)
            :"memory", "x10", "x11", "x12", "x17"
            );
    return ret;
}

void consputc(char x) {
    sbi_call(SBI_PUTCHAR, x, 0, 0);
}
//bss段清零
void sbss();
void ebss();
void clear_bss() {
    for (char *i = (char *)sbss; i < (char *)ebss; i++) *i = 0;
}
//一些函数：printf、shutdown、Panic、main
void printf(char *, ...);
void shutdown() {
    sbi_call(SBI_SHUTDOWN, 0, 0, 0);
    printf("It should shutdown!");
}
void Panic (){
    printf("Panic: It should be shutdown!\n");
    shutdown();
}
void main() {
    clear_bss();
extern char edata[], end[]; 
    printf("Hello, world!\n");
   Panic();
}

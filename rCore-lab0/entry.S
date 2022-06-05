#指定程序的 _start 入口点，栈的大小为 4096 * 16 = 64KB

    .section .text.entry                #后面的内容全部放到一个名为 .text.entry 的段中
    .globl _start                              #全局变量：定义_start入口点,# 使得ld能够看到_start这个符号所在的位置
_start:                                             #声明了一个符号 _start ，该符号指向紧跟在符号后面的内容
    la sp, boot_stack_top         #将bootstacktop的地址加载到sp(stack pointer)寄存器中, 使用我们分配的内核栈
    call main                                   #调用main, 这是我们要用C语言编写的一个函数, call是伪指令，作用是调用函数（跳转）

    .section .bss.stack                 #开始bss section(可初始化为0的可读写段)
    .globl boot_stack                   #栈底,内核栈

boot_stack:
    .space 4096 * 16                     #留出64K字节的内存
    .globl boot_stack_top         #栈顶,之后内核栈将要从高地址向低地址增长, 初始时的内核栈为空
boot_stack_top:


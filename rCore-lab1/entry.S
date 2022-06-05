   #指定程序的 _start 入口点，栈的大小为 4096 * 16 = 64KB 
    
    .section .text.entry
    .globl _start            #全局变量：定义_start入口点
_start:
    la sp, boot_stack_top
    call load_all            #入口函数

    .section .bss.stack
    .globl boot_stack        #栈底
boot_stack:
    .space 4096 * 16
    .globl boot_stack_top    #栈顶
boot_stack_top:

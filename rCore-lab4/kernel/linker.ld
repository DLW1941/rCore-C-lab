/* 指定输出文件的指令集架构, 在riscv平台上运行 */
OUTPUT_ARCH(riscv)

/* 指定程序的入口点,我们之后会在汇编代码里定义它 */
ENTRY(_start)

/* 数据存放起始地址 */
BASE_ADDRESS = 0x80200000;

/*链接脚本剩余的部分是一整条SECTIONS指令，用来指定输出文件的所有SECTION
 "." 是SECTIONS指令内的一个特殊变量/计数器，对应内存里的一个地址。*/
SECTIONS
{
    . = BASE_ADDRESS;
    /* skernel 表示全部的开始位置 */
    skernel = .;

    stext = .;
    /* .text 字段:代码段 */
    .text : {
        *(.text.entry)
        . = ALIGN(4K);
        strampoline = .;
        *(.text.trampoline);
        . = ALIGN(4K);
        *(.text .text.*)
    }

    . = ALIGN(4K);
    etext = .;
    srodata = .;
    /* .rodata 字段:只读数据段 */
    .rodata : {
        *(.rodata .rodata.*)
    }

    . = ALIGN(4K);
    erodata = .;
    /* .data 字段:存放被初始化的可读写数据 */
    sdata = .;
    .data : {
        *(.data .data.*)
    }

    . = ALIGN(4K);
    edata = .;
    sbss_with_stack = .;
    /* .bss 字段:存放被初始化为0的可读写数据*/
    .bss : {
        *(.bss.stack)
        sbss = .;
        *(.bss .bss.*)
        *(.sbss .sbss.*)
    }

    . = ALIGN(4K);
     /* 结束地址 */
    ebss = .;
    ekernel = .;

    /DISCARD/ : {
        *(.eh_frame)
    }
}

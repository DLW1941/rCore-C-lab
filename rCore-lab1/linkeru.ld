/* 目标架构 */
OUTPUT_ARCH(riscv)

/* 执行入口 */
ENTRY(_start)

/* 用户态 数据存放起始地址 */
BASE_ADDRESS = 0x80400000;

SECTIONS
{
    . = BASE_ADDRESS;

        /*代码段 */
    .text : {
        *(.text.entry)
        *(.text .text.*)
    }

        /*只读数据段 */
    .rodata : {
        *(.rodata .rodata.*)
    }

        /*存放被初始化的可读写数据 */
    .data : {
        *(.data .data.*)
    }

        /*存放被初始化为0的可读写数据*/
    .bss : {
        sbss = .;
        *(.bss .bss.*)
        ebss = .;
    }
    /DISCARD/ : {
        *(.eh_frame)
        *(.debug*)
    }
}
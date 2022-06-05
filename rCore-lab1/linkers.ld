/* 目标架构 */
OUTPUT_ARCH(riscv)

/* 执行入口 */
ENTRY(_start)

/* 数据存放起始地址 */
BASE_ADDRESS = 0x80200000;

SECTIONS
{
    /* . 表示当前地址（location counter） */
    . = BASE_ADDRESS;

    /* skernel 表示全部的开始位置 */
    skernel = .;
    stext = .;

    /* .text 字段:代码段 */
    .text : {
        /* entry 函数放在最前面 */
        *(.text.entry)
        /* 要链接的文件的 .text 字段集中放在这里 */
        *(.text .text.*)
    }

    . = ALIGN(4K);
    etext = .;


    srodata = .;
    /* .rodata 字段:只读数据段 */
    .rodata : {
        /* 要链接的文件的 .rodata 字段集中放在这里 */
        *(.rodata .rodata.*)
    }

    . = ALIGN(4K);
    erodata = .;
    sdata = .;

    /* .data 字段:存放被初始化的可读写数据 */
    .data : {
        /* 要链接的文件的 .data 字段集中放在这里 */
        *(.data .data.*)
    }

    . = ALIGN(4K);
    edata = .;

    /* .bss 字段:存放被初始化为0的可读写数据*/
    .bss : {
        *(.bss.stack)
        sbss = .;
        /* 要链接的文件的 .bss 字段集中放在这里 */
        *(.bss .bss.*)
    }

    . = ALIGN(4K);
    /* 结束地址 */
    ebss = .;
    ekernel = .;

    /DISCARD/ : {
        *(.eh_frame)
    }
}
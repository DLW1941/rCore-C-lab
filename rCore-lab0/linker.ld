/* 指定输出文件的指令集架构(目标平台), 在riscv平台上运行 */
OUTPUT_ARCH(riscv)

/* 指定程序的入口点,我们之后会在汇编代码里定义它 */
ENTRY(_start)

/* 数据存放起始地址、初始化代码被放置的地址 */
BASE_ADDRESS = 0x80200000;

/*链接脚本剩余的部分是一整条SECTIONS指令，用来指定输出文件的所有SECTION
 "." 是SECTIONS指令内的一个特殊变量/计数器，对应内存里的一个地址。
 ":"冒号前面表示最终生成的可执行文件的一个段的名字
 "{"花括号内按照放置顺序描述将所有输入目标文件的哪些段放在这个段中*/

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

README

  本项目是基于RISC-V的操作系统实验集成与改进，参考[rCore操作系统实验](https://rcore-os.github.io/rCore-Tutorial-Book-v3/index.html)设置，参考[uCore](https://nankai.gitbook.io/ucore-os-on-risc-v64/)和xv6（[中文跟做实验](https://blog.csdn.net/u013577996/article/details/108679997)、学校官网教程 [MIT 6.S081](https://pdos.csail.mit.edu/6.S081/2021/)）的部分实验代码，用C翻译Rust代码，简化实验细节，作为想学习rCore却无Rust语言基础的同学作为过渡性试验。

本项目完成了5个实验的C移植Rust代码：内核启动、简单批处理、分时多任务、内存管理、进程管理。实验代码放在了master分支的各个文件夹中，可以通过 git clone URL 下载。

  仓库中包含了做毕设过程的[实验代码](https://github.com/DLW1941/rCore-C-lab/tree/master)、[实验指导书](https://rcore-c-tuturial.gitbook.io/rcore-tutorial-c-version1/)、[论文最终稿](https://github.com/DLW1941/rCore-C-lab/blob/master/%E5%9F%BA%E4%BA%8ERISC-%E2%85%A4%E7%9A%84%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E5%AE%9E%E9%AA%8C%E9%9B%86%E6%88%90%E5%92%8C%E6%94%B9%E8%BF%9B.pdf)、[重现说明](https://github.com/DLW1941/rCore-C-lab/blob/master/%E9%87%8D%E7%8E%B0%E8%AF%B4%E6%98%8E.docx)、[总结文稿](https://github.com/DLW1941/rCore-C-lab/blob/master/%E6%80%BB%E7%BB%93.docx)、[答辩幻灯片](https://github.com/DLW1941/rCore-C-lab/blob/master/%E6%AF%95%E4%B8%9A%E8%AE%BE%E8%AE%A1%E7%AD%94%E8%BE%A9V3.0.pptx)（含软件演示视频）、[外文翻译](https://github.com/DLW1941/rCore-C-lab/blob/master/%E5%A4%96%E6%96%87%E7%BF%BB%E8%AF%91_%E5%A4%A7%E5%9E%8B%E7%B3%BB%E7%BB%9F%E8%BD%AF%E4%BB%B6%E9%83%A8%E5%88%86%E6%95%85%E9%9A%9C%E7%9A%84%E7%90%86%E8%A7%A3%E3%80%81%E6%A3%80%E6%B5%8B%E4%B8%8E%E5%AE%9A%E4%BD%8D.pdf)等。

	1.内核启动的实验，需要掌握的是内核的入口点、内核空间的内存布局、SBI的系统调用以及makefile的编写。通过汇编代码指定内核程序的加载入口；通过链接脚本指定输入输出文件之间的映射，指定它们的内存布局；SBI的系统调用涉及到寄存器操作，需了解RISC-V架构的寄存器使用以及内联汇编。

	2.简单批处理的实验，旨在掌握顺序执行、自动装载应用程序的实现。有三个主要实验内容：简单应用程序的设计，将会涉及到系统调用；用户空间与内核空间的隔离，一个简单的实现方法是分别为用户程序和内核程序用链接脚本指定内存布局，也即是静态内存布局；内核栈与用户栈的切换、陷入上下文的保存和恢复等，将涉及寄存器CSR的操作，在RISC-V汇编中体现。

	3.分时多任务的实验，掌握时间片轮转的实现机制，主要是RISC-V架构机器上关于计时钟周期的寄存器的调用、定时中断的产生；以及多道程序加载、任务切换机制和任务切换时上下文的恢复与保存。

	4.内存管理的实验，这一实验拆分成四个部分完成：一是添加链表数据结构以支持内核的动态内存分配；二是物理页帧管理，将内存空间切分成固定大小的页，设计空闲分配池的实现：分配页帧时先查询空闲分配池是否存在对应的物理页帧号，若有则优先分配空闲池内的物理页帧号，若无则返回新页，此时的空闲池相当于栈可用链表实现；三是虚拟地址管理，要掌握RISC-V架构的SV39硬件分页机制，即逻辑地址由27位（共3级页表，每级页表9位）和12位页内偏移构成，还需要掌握专门记录虚实地址映射关系以及访问信息的页表项PTE（由44位物理页号和8位标志位构成）的使用，虚拟地址正是通过PTE来映射物理地址；四是分时多任务系统与内存管理的融合，为任务分配地址空间时不再是连续的段地址空间，而是分页的，需要保证在任务切换时任务上下文的保存和恢复，以及任务执行完后回收相应的物理页帧地址空间。

	5.进程管理的实验， 这一部分将实现与进程创建相关的函数如initproc、fork、exec。前两者都能实现从零到有的进程创建，initproc是用户初始进程，是唯一由内核硬编码形式实现的进程，其余所有进程由fork创建，但fork创建的进程只能和父进程一样执行同样的代码段；exec可复用父进程的模块，并执行与父进程不同的可执行文件。其次是实现用户命令行user_shell，捕获用户输入，并根据输入执行相应的进程，进程执行结束后返回相应的进程标识数PID。



#include "kernel.h"

PhysPageNum kernel_pagetable;
//取页表项：idx可取出虚拟页号的三级页索引，取出低27位
PageTableEntry *find_pte(PhysPageNum root, VirtPageNum vpn, int create) {
    usize idx[3];
    for (int i = 2; i >= 0; i--) {
        idx[i] = vpn & 511; vpn >>= 9;//511=111111111,取位操作；右移赋值；
//假如只取一位，256=100000000，会出现映射错误的情况
    }
    for (int i = 0; i < 3; i++) {
        PageTableEntry *pte_p = (PageTableEntry *)PPN2PA(root) + idx[i];
        if (i == 2) return pte_p;
        if (!(*pte_p & V)) {
            if (! create) panic("find_pte failed");
            PhysPageNum frame = frame_alloc();
            *pte_p = PPN2PTE(frame, V);
        }
        root = PTE2PPN(*pte_p);//页表的起始地址、页表根节点的地址
    }
    return 0;
}
//建立虚实地址映射，改写自page_table.rs
void map(PhysPageNum root, VirtPageNum vpn, PhysPageNum ppn, PTEFlags flags) {
    PageTableEntry *pte_p = find_pte(root, vpn, 1);
    if (*pte_p & V)
        panic("map: vpn is mapped before mapping!");
    *pte_p = PPN2PTE(ppn, flags | V);
}
//拆除虚实地址映射关系
PhysPageNum unmap(PhysPageNum root, VirtPageNum vpn) {
    PageTableEntry *pte_p = find_pte(root, vpn, 1);
    if (!(*pte_p & V))
        panic("unmap: vpn is invalid before unmapping");
    PhysPageNum ppn = PTE2PPN(*pte_p);
    *pte_p = 0; return ppn;
}
//alloc参数为0映射内核空间，这种情况下虚拟页号等于物理页号
//alloc参数为1映射用户空间，这种情况随映射随创建就行
void map_area(PhysPageNum root, VirtAddr start_va, VirtAddr end_va, PTEFlags flags, int alloc) {
    VirtPageNum start_vpn = FLOOR(start_va), end_vpn = CEIL(end_va);
    for (VirtPageNum i = start_vpn; i < end_vpn; i++) {
        PhysPageNum ppn = alloc ? frame_alloc() : i;
        map(root, i, ppn, flags);
    }
}
//解映射时物理空间要保留，对应的物理页帧释放掉
void unmap_area(PhysPageNum root, VirtAddr start_va, VirtAddr end_va, int dealloc) {
    VirtPageNum start_vpn = FLOOR(start_va), end_vpn = CEIL(end_va);
    for (VirtPageNum i = start_vpn; i < end_vpn; i++) {
        PhysPageNum ppn = unmap(root, i); if (dealloc) frame_dealloc(ppn);
    }
}
//主要实现虚拟地址里的数据和物理地址里的数据传递
//to_va为1，将data里的数据传给root页表对应的start_va虚拟地址开始的空间
//to_va为0，将root页表对应的start_va虚拟地址开始的数据传给data
void copy_area(PhysPageNum root, VirtAddr start_va, void *data, int len, int to_va) {
    char *cdata = (char *)data; VirtPageNum vpn = FLOOR(start_va);
    while (len) {
        usize frame_off = start_va > PPN2PA(vpn) ? start_va - PPN2PA(vpn) : 0;
        usize copy_len = PAGE_SIZE - frame_off < len ? PAGE_SIZE - frame_off : len;
        PageTableEntry *pte_p = find_pte(root, vpn, 0);
        if (to_va) memcpy((void *)PPN2PA(PTE2PPN(*pte_p)) + frame_off, cdata, copy_len);
        else memcpy(cdata, (void *)PPN2PA(PTE2PPN(*pte_p)) + frame_off, copy_len);
        len -= copy_len; cdata += copy_len; vpn++;
    }
}
//映射跳表：改写自config.rs
void map_trampoline(PhysPageNum root) {
    extern char strampoline;
    map(root, FLOOR(TRAMPOLINE), FLOOR((PhysAddr)&strampoline), R | X);
}
//递归释放页表
void free_pagetable(PhysPageNum root) {
    PageTableEntry *pte_p = (PageTableEntry *)PPN2PA(root);
    for (int i = 0; i < 512; i++) {
        if (pte_p[i] & V) {
            if (!(pte_p[i] & R) && !(pte_p[i] & W) && !(pte_p[i] & X))
                free_pagetable(PTE2PPN(pte_p[i]));
        }
    }
    frame_dealloc(root);
}
//内核地址空间初始化
void kvm_init() {
    frame_init();
    kernel_pagetable = frame_alloc();
    extern char stext, etext, srodata, erodata, sdata, edata,
           sbss_with_stack, ebss, ekernel;
    map_trampoline(kernel_pagetable);
    map_area(kernel_pagetable, (VirtAddr)&stext, (VirtAddr)&etext, R | X, 0);
    map_area(kernel_pagetable, (VirtAddr)&srodata, (VirtAddr)&erodata, R, 0);
    map_area(kernel_pagetable, (VirtAddr)&sdata, (VirtAddr)&edata, R | W, 0);
    map_area(kernel_pagetable, (VirtAddr)&sbss_with_stack, (VirtAddr)&ebss, R | W, 0);
    map_area(kernel_pagetable, (VirtAddr)&ekernel, MEMORY_END, R | W, 0);
    usize satp = PGTB2SATP(kernel_pagetable);
    asm volatile ("csrw satp, %0\nsfence.vma"::"r"(satp));
}

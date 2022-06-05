CC = riscv64-unknown-elf-gcc -ffreestanding -nostdlib -g -mcmodel=medany -Icommon
OC = riscv64-unknown-elf-objcopy --strip-all -O binary

default: user
	$(CC) kernel/*.c kernel/*.S common/common.c -T kernel/linker.ld -o build/os
	$(OC) build/os build/os.bin
	qemu-system-riscv64 -machine virt -nographic -bios common/rustsbi-qemu.bin \
		-device loader,file=build/os.bin,addr=0x80200000

debug: user
	$(CC) kernel/*.c kernel/*.S common/common.c -T kernel/linker.ld -o build/os
	$(OC) build/os build/os.bin
	qemu-system-riscv64 -machine virt -nographic -bios common/rustsbi-qemu.bin \
		-device loader,file=build/os.bin,addr=0x80200000 -s -S

user:
ifneq (build, $(wildcard build))
	mkdir build
endif
	$(CC) user/task0.c user/lib.c common/common.c -T user/linker.ld -o build/task0
	$(CC) user/task1.c user/lib.c common/common.c -T user/linker.ld -o build/task1
	$(CC) user/task2.c user/lib.c common/common.c -T user/linker.ld -o build/task2


clean:
	rm build/*
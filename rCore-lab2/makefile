CC = riscv64-unknown-elf-gcc -ffreestanding -nostdlib -g -mcmodel=medany
OC = riscv64-unknown-elf-objcopy --strip-all -O binary
default: user
	$(CC) task.c loader.c mod.c timer.c printf.c syscall.c sbicall.c \
		entry.S link_app.S trap.S switch.S \
		-T linkers.ld -o os
	$(OC) os os.bin
	qemu-system-riscv64 -machine virt -nographic -bios rustsbi-qemu.bin \
		-device loader,file=os.bin,addr=0x80200000

debug: default
	qemu-system-riscv64 -machine virt -nographic -bios rustsbi-qemu.bin \
		-device loader,file=os.bin,addr=0x80200000 -s -S
		
user:
	$(CC) task0.c lib.c -T linkeru0.ld -o task
	$(OC) task task0.bin
	$(CC) task1.c lib.c -T linkeru1.ld -o task
	$(OC) task task1.bin
	$(CC) task2.c lib.c -T linkeru1.ld -o task
	$(OC) task task2.bin

clean:
	$(if $(wildcard task0),  rm task0)
	$(if $(wildcard task0.bin), rm task0.bin)
	$(if $(wildcard task1), rm task1)
	$(if $(wildcard task1.bin), rm task1.bin)
	$(if $(wildcard task2), rm task2)
	$(if $(wildcard task2.bin), rm task2.bin)
	$(if $(wildcard os), rm os)
	$(if $(wildcard os.bin), rm os.bin)


default: user_print.bin
	riscv64-unknown-elf-gcc batch.c mod.c printf.c syscall.c sbicall.c entry.S link_app.S trap.S \
		-T linkers.ld -ffreestanding -nostdlib -g -mcmodel=medany \
		-o os
	riscv64-unknown-elf-objcopy os --strip-all -O binary os.bin
	qemu-system-riscv64 -machine virt -nographic -bios rustsbi-qemu.bin \
		-device loader,file=os.bin,addr=0x80200000

debug: user_print.bin
	riscv64-unknown-elf-gcc batch.c mod.c printf.c syscall.c sbicall.c entry.S link_app.S trap.S \
		-T linkers.ld -ffreestanding -nostdlib -g -mcmodel=medany \
		-o os
	riscv64-unknown-elf-objcopy os --strip-all -O binary os.bin
	qemu-system-riscv64 -machine virt -nographic -bios rustsbi-qemu.bin \
		-device loader,file=os.bin,addr=0x80200000 -s -S

user_print.bin:
	riscv64-unknown-elf-gcc user_print.c lib.c \
		-T linkeru.ld -ffreestanding -nostdlib -g -mcmodel=medany \
		-o user_print
	riscv64-unknown-elf-objcopy user_print --strip-all -O binary user_print.bin

clean:
	$(if $(wildcard user_print), rm user_print)
	$(if $(wildcard user_print.bin), rm user_print.bin)
	$(if $(wildcard os), rm os)
	$(if $(wildcard os.bin), rm os.bin)


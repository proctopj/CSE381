all: os-image

run: all
	qemu-system-i386

# $^ is all the dependency files
os-image: boot_sect.bin kernel.bin
	cat $^ > os-image.tmp
	dd if=/dev/zero of=$@ bs=512 count=200
	dd if=os-image.tmp of=$@ conv=notrunc
	rm os-image.tmp

kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -s -Ttext 0x1000 --oformat binary -o kernel.bin $^

# $< is the first dependency file, $@ is the target file
kernel.o: kernel.c screen.c screen.h
	gcc -m32 -ffreestanding -o $@ -c $<

kernel_entry.o: kernel_entry.asm
	nasm -f elf -o $@ kernel_entry.asm

boot_sect.bin: boot_sect.asm
	nasm -o $@ -f bin $<

run:
	qemu-system-i386 os-image

clean:
	rm -rf *.o *.bin *.dis *.map os-image

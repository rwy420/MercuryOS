.set MAGIC_BOOT, 0x1badb002
.set FLAGS,	(1 << 0 | 1 << 1)
.set CHECKSUM, -(MAGIC_BOOT + FLAGS)

# Include values in object file
.section .multiboot
	.long MAGIC_BOOT
	.long FLAGS
	.long CHECKSUM

.section .text

.extern KernelMain

.extern kernel_virtual_start
.extern kernel_virtual_end
.extern kernel_physical_start
.extern kernel_physical_end

.global boot

boot:
	# Set stack pointer
	mov $stack, %esp
	# Push values for kernel entry
	push kernel_physical_end
	push kernel_physical_start
	push kernel_virtual_end
	push kernel_virtual_start
	push %ebx



	call KernelMain

# Should not get to this point
_halt:
	cli
	hlt
	jmp _halt

.section .bss

.space 4*1024*1024 
stack:

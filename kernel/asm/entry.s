.extern KernelMain

.extern kernel_virtual_start
.extern kernel_virtual_end
.extern kernel_physical_start
.extern kernel_physical_end

.global boot

boot:
	# Set stack pointer
	mov $stack, %esp

	call KernelMain

# Should not get to this point
_halt:
	cli
	hlt
	jmp _halt

.section .bss

.space 4*1024*1024 
stack:

.section .text

.extern interrupt_handler

.macro handle_irq num
.global handle_irq_\num
handle_irq_\num:
	movb $\num, (interruptnumber)
	pushl $0 
	jmp int_common
.endm

handle_irq 00
handle_irq 01
handle_irq 02
handle_irq 03
handle_irq 04
handle_irq 05
handle_irq 06
handle_irq 07
#handle_irq 08
#handle_irq 09
handle_irq 10
handle_irq 11
handle_irq 12
handle_irq 13
handle_irq 14
handle_irq 15
handle_irq 32
handle_irq 33
handle_irq 49

int_common:
	pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    push (interruptnumber)
    call interrupt_handler

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp

	iret


.data
interruptnumber: .byte 0


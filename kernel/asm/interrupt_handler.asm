section .text

extern interrupt_handler

%macro no_error_interrupt_handler 1
global handle_irq_%1
handle_irq_%1:
	;#push dword 0
	push dword %1
	jmp common_interrupt_handler
%endmacro

%macro error_interrupt_handler 1
global handle_irq_%1
handle_irq_%1:
	push dword %1
	jmp common_interrupt_handler
%endmacro

common_interrupt_handler:
	;#push eax
	;#push ecx
	;#push edx
	;#push ebx
	;#push esp
	;#push ebp
	;#push esi
	;#push edi

	call interrupt_handler

	;#pop edi
	;#pop esi
	;#pop ebp
	;#pop esp
	;#pop ebx
	;#pop edx
	;#pop ecx
	;#pop eax

	add esp, 4

	iret

no_error_interrupt_handler 00
no_error_interrupt_handler 01
no_error_interrupt_handler 02
no_error_interrupt_handler 03
no_error_interrupt_handler 04
no_error_interrupt_handler 05
no_error_interrupt_handler 06
no_error_interrupt_handler 07
no_error_interrupt_handler 08
no_error_interrupt_handler 09
no_error_interrupt_handler 10
no_error_interrupt_handler 11
no_error_interrupt_handler 12
no_error_interrupt_handler 13
no_error_interrupt_handler 14	
no_error_interrupt_handler 15
no_error_interrupt_handler 32
no_error_interrupt_handler 33
no_error_interrupt_handler 49

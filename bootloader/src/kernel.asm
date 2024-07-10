org 0x8000
[bits 32]

%define ENDL 0x0D, 0x0A

main:
	mov eax, 0x200

.halt
	jmp .halt

puts:
	push esi
	push eax


.loop:
	lodsb
	or al, al
	jz .done

	mov ah, 0x0E
	int 0x10

	jmp .loop

.done:
	pop eax
	pop esi
	ret

msg_loading: db "Kernel", ENDL, 0

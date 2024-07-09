org 0x0
bits 16

%define ENDL 0x0d, 0x0a

start:
	mov si, msg_hello
	call puts

.halt:
	cli
	hlt

puts:
	push si
	push ax

.loop:
	lodsb
	or al, al
	jz .done

	mov ah, 0x0e
	int 0x10

	jmp .loop

.done:
	pop ax
	pop si
	ret

msg_hello: db "Boot Stage 2", ENDL, 0

org 0x7C00
bits 16

%define ENDL 0x0D, 0x0A

bdb_bytes_per_sector:       dw 512
bdb_sectors_per_track:      dw 18
bdb_heads:                  dw 2
drive_number:				db 0

start:
	mov ax, 0
	mov ds, ax
	mov es, ax

	mov ss, ax
	mov sp, 0x7C00

	push es
	push word .after
	retf

.after:
	mov [drive_number], dl

	mov si, msg_loading
	call puts

	;push es
	;mov ah, 08h
	;int 13h
	;jc disk_error
	;pop es

	;call read_boot_sector

	;mov ax, 0x42
	;mov dl, [drive_number]
	;mov cl, 1
	;mov bx, buffer

; --> ax: lba address
; --> cl: number of sectors to read
; --> dl: drive number
; --> es:bx memory address where to store read data

.halt:
	cli
	hlt

; --> si: pointer to messgae
puts:
	push si
    push ax
    push bx

.loop:
    lodsb          
	or al, al           
	jz .done

    mov ah, 0x0E
	mov bh, 0
	int 0x10

    jmp .loop

.done:
    pop bx
    pop ax
    pop si    
    ret

; --> ax: lba address
; <-- cx bits 0 - 5: sector number
; <-- cx bits 5 - 15: cylinder
; <-- dh: head
lba_to_chs:

    push ax
    push dx

    xor dx, dx
	div word [bdb_sectors_per_track]

    inc dx
    mov cx, dx

    xor dx, dx
    div word [bdb_heads]

    mov dh, dl     
    mov ch, al            
    shl ah, 6
    or cl, ah                           

    pop ax
    mov dl, al 
    pop ax
    ret
	
; --> ax: lba address
; --> cl: number of sectors to read
; --> dl: drive number
; --> es:bx memory address where to store read data
disk_read:
    push ax
	push bx
    push cx
    push dx
    push di

    push cx
	call lba_to_chs 
	pop ax

    mov ah, 02h
    mov di, 3 

.retry:
    pusha
	stc
	int 13h
	jnc .done

    popa
    call disk_reset

    dec di
    test di, di
    jnz .retry

.fail:
    jmp disk_error

.done:
    popa

    pop di
    pop dx
    pop cx
    pop bx
    pop ax
	ret


; --> dl: driver number
disk_reset:
    pusha
    mov ah, 0
    stc
    int 13h
    jc disk_error
	popa
    ret

disk_error:
	mov si, msg_disk_error
	call puts
	jmp reboot

reboot:
	jmp 0FFFFh:0

msg_loading: db "Loading...", ENDL, 0
msg_disk_error: db "Disk error",  ENDL, 0

STAGE2_LOAD_SEGMENT equ 0x2000

times 510-($-$$) db 0
dw 055AAh

buffer:

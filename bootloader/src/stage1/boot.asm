org 0x7c00
bits 16

%define ENDL 0x0d, 0x0a

jmp short start
nop

bdb_oem:					db "MSWIN4.1"
bdb_bytes_per_sector:		dw 512
bdb_sectors_per_cluster:	db 1
bdb_reserved_sectors:		dw 1
bdb_fat_count:				db 2
bdb_dir_entries_count:		dw 0e0h
bdb_total_sectors:			dw 2880
bdb_media_descriptor_type:	db 0f0h
bdb_sectors_per_fat:		dw 9
bdb_sectors_per_track:		dw 18
bdb_heads:					dw 2
bdb_hidden_sectors:			dd 0
bdb_large_sector_count:		dd 0

ebr_drive_number:			db 0
							db 0
ebr_signature:				db 29h
ebr_volume_id:				db 12h, 34h, 56h, 78h
ebr_volume_label:			db "MRCRYBOOT  "
ebr_system_id:				db "FAT12   "

start:
	mov ax, 0
	mov dx, ax
	mov es, ax

	mov ss, ax
	mov sp, 0x7c00

	push es
	push word .after
	retf

.after:	

	mov [ebr_drive_number], dl


	mov si, msg_hello
	call puts

	push es
	mov ah, 08h
	int 13h
	jc disk_error
	pop es

	and cl, 0x3f
	xor ch, ch
	mov [bdb_sectors_per_track], cx

	inc dh
	mov [bdb_heads], dh

	mov ax, [bdb_sectors_per_fat]
	mov bl, [bdb_fat_count]
	xor bh, bh
	mul bx
	add ax, [bdb_reserved_sectors]
	push ax

	mov ax, [bdb_sectors_per_fat]
	shl ax, 5
	xor dx, dx
	div word [bdb_bytes_per_sector]

	test dx, dx
	jz .root_dir_after
	inc ax

.root_dir_after:
	mov cl, al
	pop ax
	mov dl, [ebr_drive_number]
	mov bx, buffer
	call disk_read

	xor bx, bx
	mov di, buffer

.search_stage2:
	mov si, file_stage2
	mov cx, 11
	push di
	repe cmpsb
	pop di
	je .found_stage2

	add di, 32
	inc bx
	cmp bx, [bdb_dir_entries_count]
	jl .search_stage2

	jmp disk_error

.found_stage2:
	mov ax, [di + 26]
	mov [stage2_cluster], ax

	mov ax, [bdb_reserved_sectors]
	mov bx, buffer
	mov cl, [bdb_sectors_per_fat]
	mov dl, [ebr_drive_number]
	call disk_read

	mov bx, STAGE2_LOAD_SEGMENT
	mov es, bx
	mov bx, STAGE2_LOAD_OFFSET

.load_stage2_loop:
	mov ax, [stage2_cluster]
	add ax, 31

	mov cl, 1
	mov dl, [ebr_drive_number]
	call disk_read

	add bx, [bdb_bytes_per_sector]

	mov ax, [stage2_cluster]
	mov cx, 3
	mul cx
	mov cx, 2
	div cx

	mov si, buffer
	add si, ax
	mov ax, [ds:si]

	or dx, dx
	jz .even

.odd:
	shr ax, 4
	jmp .next_cluster_after

.even:	
	and ax, 0x0fff

.next_cluster_after:
	cmp ax, 0x0ff8
	jae .read_finish

	mov [stage2_cluster], ax
	jmp .load_stage2_loop

.read_finish:	
	mov dl, [ebr_drive_number]
	mov ax, STAGE2_LOAD_SEGMENT
	mov ds, ax
	mov es, ax

	jmp STAGE2_LOAD_SEGMENT:STAGE2_LOAD_OFFSET

	jmp wait_key_reboot

	cli
	hlt

disk_error:
	mov si, msg_read_fail
	call puts
	jmp wait_key_reboot

wait_key_reboot:
	mov ah, 0
	int 16h
	jmp 0FFFFh:0

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

; --> ax: LBA address
; <-- cx bits 0 - 5: Sector
; <-- cx bits 5 - 15: Cylinder
; <-- dh: Head

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

; --> ax: LBA address
; --> cl: Number of sectors to read
; --> dl: Drive number
; --> es:bx: Memory address where to store read data

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

.read_fail:
	jmp disk_error

.done:
	popa

	pop di
	pop dx
	pop cx
	pop bx
	pop ax

	ret

; --> dl: Drive number

disk_reset:
	pusha
	mov ah, 0
	stc
	int 13h
	jc disk_error
	popa
	ret

msg_hello: db "OK", ENDL, 0
msg_read_fail: db "E1", ENDL, 0

file_stage2: db "STAGE2  BIN"

stage2_cluster: dw 0

STAGE2_LOAD_SEGMENT equ 0x2000
STAGE2_LOAD_OFFSET equ 0

;times 510-($-$$) db 0
;dw 0aa55h

times 440-($-$$) db 0

buffer:

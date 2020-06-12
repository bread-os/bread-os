section .boot
bits 16
global boot
boot:
	mov ax, 0x2401
	int 0x15
	mov ax, 0x3
	int 0x10

	mov [disk], dl
	mov ah, 0x2		; read sectors
	mov al, 6		; sectors to read
	mov ch, 0		; cylinder idx
	mov dh, 0		; head idx
	mov cl, 2		; sector idx
	mov dl, [disk]	; disk idx
	mov bx, copy_target	; target pointer
	int 0x13

	cli
	lgdt [gdt_pointer]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp CODE_SEG:boot2
gdt_start:
	dq 0x0
gdt_code:
	dw 0xffff
	dw 0x0
	db 0x0
	db 10011010b
	db 11001111b
	db 0x0
gdt_data:
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0
gdt_end:
gdt_pointer:
	dw gdt_end - gdt_start
	dd gdt_start
disk:
	db 0x0
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

times 510 - ($-$$) db 0
dw 0xaa55	; magic number
copy_target:
bits 32
	mbr-load: db "[bread-os:log] MBR load success.", 0
boot2:
	mov esi, mbr-load
	mov ebx, 0xb8000
.loop:
	lodsb
	or al,al
	jz halt
	or eax, 0x0f00
	mov word[ebx], ax
	add ebx, 2
	jmp .loop
halt:
	mov esp, kernel_stack_top
	extern kmain
	call kmain
	cli
	hlt

section .bss
align 4
kernel_stack_bottom: equ $
	resb 16384 ; 16 KB
kernel_stack_top:

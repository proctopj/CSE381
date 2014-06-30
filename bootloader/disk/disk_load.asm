disk_load:
push dx

mov ah, 0x02    ; BIOS read sector
mov al, dh      ; How many sectors to read?
mov ch, 0x00    ; Cylinder 0
mov dh, 0x00    ; Head 0
mov cl, 0x02    ; Start from second sector (after boot sector)

int 0x13

jc disk_error   ; Jump if carry flag is set

pop dx

cmp al, dh      ; See if actually read in al is equal to expected
jne disk_error
ret

disk_error:
mov bx, DISK_ERROR_MESSAGE
call print_string
jmp $

DISK_ERROR_MESSAGE:
db "Error reading disk", 0

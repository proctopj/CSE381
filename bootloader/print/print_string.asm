print_string:
pusha           ; Save all

print_loop:
mov cx, [bx]    ; Copy in order to
cmp cl, 0       ; see if null terminator
je print_end    ; Jump if cl == 0

mov ax, [bx]    ; Store content of address BX
mov ah, 0x0e
int 0x10
add bx, 1       ; Increment address to look at
jmp print_loop

print_end:
popa
ret

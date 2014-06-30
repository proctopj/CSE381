; Function that prints out a hex {{{
print_hex:
mov bx, HEX_OUT
add bx, 2           ; Skip "0x"

; First
mov al, dh
shr al, 4           ; Get first number
call char_ize
mov [bx], al        ; Set to the char
add bx, 1

; Second
mov al, dh          ; Copy for mask
and al, 0x0f        ; Get least significant 4 bits (Second number)
call char_ize
mov [bx], al
add bx, 1

; Third
mov al, dl
shr al, 4           ; Get third number
call char_ize
mov [bx], al        ; Set to the char
add bx, 1

; Fourth
mov al, dl          ; Copy for mask
and al, 0x0f        ; Get least significant 4 bits (Fourth number)
call char_ize
mov [bx], al

; Print
mov bx, HEX_OUT
call print_string
ret
; }}}

char_ize:           ; Convert number in al register to char {{{
cmp al, 0x9
jg alpha            ; If al > 9, it's an alphabet

; Else, it's a number (0-9)
add al, 0x30        ; Add '0'
jmp end

alpha:              ; If al is a-f
sub al, 0xa         ; Offset from 0xa
add al, 0x61        ; Add 'a'

end:
ret
; }}}

HEX_OUT:
db '0x0000', 0

[bits 16]

switch_to_pm:
cli
; Switch to...
lgdt [gdt_descriptor]
; 32 bit protected mode!
mov eax, cr0
or eax, 0x1
mov cr0, eax

jmp CODE_SEG:init_pm    ; Far jump (jmp to a new segment) in order to flush
                        ; CPU cache of pre-fetched and 16 bit real-mode
                        ; instructions
[bits 32]

init_pm:

mov ax, DATA_SEG        ; In Protected Mode, old segments are meaningless
mov ds, ax              ; so point segment registers to the data selector
mov ss, ax              ; defined in GDT
mov es, ax
mov fs, ax
mov gs, ax

mov ebp, 0x90000        ; Update stack position so it's right
mov esp, ebp            ; at the top of the free space

call BEGIN_PM

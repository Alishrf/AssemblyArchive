%include "asm_io.inc"
section .bss
l1: dd 0
len_l1: dd 0
l2: dd 0
len_l2: dd 0
l3: dd 0
section .text
global correlate

correlate:
    mov eax , [esp + 4]
    mov [l1] , eax
    mov eax , [esp + 8]
    mov [len_l1] , eax
    mov eax , [esp + 12]
    mov [l2] , eax
    mov eax , [esp + 16]
    mov [len_l2] , eax
    mov eax , [esp + 20]
    mov [l3] , eax
    mov ecx,0 
loop1:
    fldz
    mov edx,0
loop2:
    mov ebx,[len_l2]
    shr ebx,1
    mov eax , [len_l2]
    sub eax , ebx
    add eax , ecx
    cmp eax,0
    jge next
    cmp eax,[len_l1]
    jl next
    jmp end_next
next:    
    fld dword [l1 + 4*eax]
    fmul dword [l2 + 4*edx]
    fadd st1

end_next:
    ;------
    inc edx
    cmp edx , len_l2
    jnz loop2

;---
    fst dword [l3 + 4*ecx]
;---

    inc ecx
    cmp ecx , len_l1
    jnz loop1    
    

ret

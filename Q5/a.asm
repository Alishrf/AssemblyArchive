%include "asm_io.inc"
segment .data
l1: dd 0
len_l1: dd 0
l2: dd 0
len_l2: dd 0
l3: dd 0
zeroo: dd 0.0
alaki: dd 0.0
var: dd 0.0
segment .text
global correlate

correlate:
    enter 0 ,0
    pusha
    mov eax , [ebp + 8]
    mov [l1] , eax
    mov eax , [ebp + 12]
    mov [len_l1] , eax
    mov eax , [ebp + 16]
    mov [l2] , eax
    mov eax , [ebp + 20]
    mov [len_l2] , eax
    mov eax , [ebp + 24]
    mov [l3] , eax
    mov ecx,0 
loop1:
    fstp dword [alaki]
    fld dword [zeroo]
    mov edx,0
loop2:
    mov ebx,[len_l2]
    shr ebx,1
    mov eax , edx
    sub eax , ebx
    add eax , ecx
    cmp eax,0
    jge next
    jmp end_next
next:  
    cmp eax,[len_l1]
    jl next2
    jmp end_next  
next2:    
    fld dword [var]
    mov ebx , [l1]
    fld dword [ebx + 4*eax]
    mov ebx , [l2]
    fmul dword [ebx+ 4*edx]
    fadd st1
    fstp dword [var]
    fstp dword [alaki]
    fstp dword [alaki]
end_next:
    ;------
    inc edx
    cmp edx , [len_l2]
    jne loop2

;---
     
    mov eax,[l3]
    fstp dword [eax + 4*ecx]
    fstp dword [alaki]
    fstp dword [alaki]


;---

    inc ecx
    cmp ecx , [len_l1]
    jnz loop1    
    
    
    popa
    leave
    ret
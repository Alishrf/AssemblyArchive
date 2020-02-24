%include "asm_io.inc"
section .data
ll1: dd 0
ll2: dd 0

global merge
section .text
merge:
    mov eax , [esp + 4]
    mov ebx , [esp + 8]
    add eax , [esp + 16] 
    add eax , [esp + 16] 
    add eax , [esp + 16] 
    add eax , [esp + 16] 
    add ebx , [esp + 20]
    add ebx , [esp + 20]
    add ebx , [esp + 20]
    add ebx , [esp + 20]
    mov eax , [eax]
    mov ebx , [ebx]
    cmp eax , ebx
    jg b_isSmall
    cmp eax , 0
    je b_isSmall
    mov ebx , [esp + 12]
    add ebx , [esp + 24]
    add ebx , [esp + 24]
    add ebx , [esp + 24]
    add ebx , [esp + 24]
    mov [ebx],eax
    mov eax , 0
    ret
b_isSmall:
    mov eax , [esp + 12]
    add eax , [esp + 24]
    add eax , [esp + 24]
    add eax , [esp + 24]
    add eax , [esp + 24]
    mov [eax],ebx
    mov eax , 1
    ret
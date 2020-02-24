%include "asm_io.inc"

global readArray , printArray
section .text
readArray:
    push ebp
    mov ebp , esp
    push ebx
    push ecx
    mov ebx , [ebp + 8]
    mov ecx ,0
next_inpout:
    call read_int
    cmp eax , 0
    je end_read1
    mov [ebx + ecx * 4 ] , eax
    inc ecx
    jmp next_inpout
end_read1:
    mov [ebx + ecx * 4 ] , eax
    mov eax , ecx
    pop ecx
    pop ebx
    pop ebp
    ret 4

printArray:
    mov edx , [esp + 4]
    mov ecx , [esp + 8]
    mov ebx , 0
loop1:
    mov eax ,[edx + ebx *4]
    call print_int
    mov eax , " " 
    call print_char
    inc ebx
    cmp ebx , ecx
    jne loop1
    ret 8
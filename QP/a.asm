%include "asm_io.inc"
segment .data
align 16
input: dd 1.0,2.0,3.0,4.0,5.0,6.0,7.0
len_input: dd 0
align 16
filter: dd 1.0,2.0,3.0,4.0,5.0
len_filter: dd 0
align 16
output: dd 0
align 16

var: dd 0
align 16

final: dd 0
align 16
test: dd 1.1,2.2,3.3,4.4,5.5
align 16
buffer : dd 0.0,0.0,0.0,0.0
segment .text
global correlate
extern SSEHorizontalSum32

correlate:
    enter 0 ,0
    pusha
    mov eax , [ebp + 8]
    ;mov [input] , eax    
    mov eax,[eax]
    mov eax , [ebp + 12]
    mov [len_input] , eax
    mov eax , [ebp + 16]
    ;mov [filter] , eax
    mov eax , [ebp + 20]
    mov [len_filter] , eax
    mov eax , [ebp + 24]
    mov [output] , eax

    ;-------
    mov eax , [input]
    mov ebx , [len_input]
    fldz 
    fst dword [eax + 4*ebx]
    inc ebx
    fst dword [eax + 4*ebx]
    inc ebx
    fstp dword [eax + 4*ebx]


    mov ecx,0 ; ecx = i
loop1:
    fldz
    mov edx,0 ; edx = j
loop2:
    mov ebx,[len_filter]
    shr ebx,1
    mov eax , edx ; eax = j
    sub eax , ebx ;eax = real
    add eax , ecx ; eax = real + i
    cmp eax,0     ; 
    jge next
    jmp end_next
next:  
    cmp eax,[len_input]
    jl next2
    jmp end_next  
next2:
    mov ebx,edx
    add ebx,3 ; ebx :j +3 
    cmp ebx , [len_filter]
    jge next3
    ;mov ebx , [input]
    movaps xmm0 , [input]
    ;mov ebx,[filter]
    movaps xmm1 , [filter]
    mulps xmm0,xmm1
    fstp dword [final]  
    

	movaps [buffer], xmm0
    fldz

    fld dword [buffer]
    faddp st1

    fld dword [buffer+4]
    faddp st1

    fld dword [buffer+8]
    faddp st1

    fld dword [buffer+12]
    faddp st1







    fld dword [final]
    fadd st1
    add edx , 3
    jmp end_next
next3: 
    mov ebx,[input]
    fld dword [ebx + 4*eax]
    mov ebx,[input]
    fmul dword [ebx + 4*edx]
    fstp dword [var]
    fadd dword [var] 

end_next:
    ;------
    inc edx
    cmp edx , [len_filter]
    jne loop2

;---
    mov ebx,[output]
    fstp dword [ebx + 4*ecx]

;---

    inc ecx
    cmp ecx , [len_input]
    jnz loop1    
    
    
    popa
    leave
    ret
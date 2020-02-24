%include "asm_io.inc"

%define Input_length [ebp+12]
%define Filter_length [ebp+20]
; %define Output_array [ebp + 24]
; %define Filter_array [ebp + 16]


segment .data
; input_array: resd 1000
; filter_array: resd 1000
; output_array: resd 1000
hold: resd 1
my_array: dd 0.0 

real: resd 10
segment .text


global correlate

correlate:

    enter 0 ,0
    pusha

    ; mov eax , [ebp+8]
    ; mov dword [input_array] , eax
    ; mov eax , [ebp+16]
    ; mov dword [filter_array] ,eax
    ; mov eax , [ebp +24]
    ; mov dword [output_array] , eax
    mov esi ,[ebp+8]                            ;esi =input array
    mov edi ,[ebp+16]                           ;edi=filter


    mov eax,0
    mov ebx,0


    mov ecx , 0                                                 ;ecx is "i"
outer_loop:
    cmp ecx , Input_length
    jge end_outer_loop
    fld dword [my_array]
    ; fst st1                                                     ;st1 is res

    mov edx , 0                                                 ;edx is "j"
inner_loop:
    cmp edx , Filter_length
    jge end_inner_loop

    mov eax , Filter_length
    shr eax ,1                                                 ;taqsim bar 2
    mov ebx , edx

    sub ebx , eax
    
    add ebx , ecx                                              ;ebx is "real+i"                          
    cmp ebx , 0
    jl if
    cmp ebx , Input_length
    jge if
    jmp else

if:
    ;nothing

    jmp end_else

else:
    fld dword [esi + 4*ebx]
    fmul dword [edi + 4*edx]
    fstp dword [hold]

    fadd dword [hold]    
  
end_else:
    
    add edx , 1
    jmp inner_loop
end_inner_loop:

    mov ebx, [ebp+24]
    fst dword [ebx + 4*ecx]

    
    add ecx , 1
    jmp outer_loop
end_outer_loop:




    
    popa        ;protect the registers
    leave
    ret         ;end 
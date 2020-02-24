section .bss

input: dd 0
input_length: dd 0
filter: dd 0
filter_length: dd 0
output: dd 0
res: dd 0

section .text
global correlate
extern SSEHorizontalSum32

correlate:

enter 0,0


    mov eax , [ebp+8]   
    mov [input] , eax   ;read input
    
    mov eax , [ebp+12]
    mov [input_length] , eax 
    
    mov eax , [ebp+16]
    mov [filter] , eax  ;read filter
    
    mov eax , [ebp+20]
    mov [filter_length] , eax   ;read filter_lenght

    mov eax , [ebp+24]
    mov [output] , eax  ;read output
    
    mov eax,[input_length]
    mov dword [input + eax*4], 0
    
    mov eax,[filter_length]
    mov dword [filter + eax*4], 0
    
    mov eax,[input_length]
    mov dword [output + eax*4], 0
    mov ecx, 0
    
    xorps xmm2,xmm2
outerLoop: ;i = ecx

    cmp ecx, [input_length]
    jge done
    mov ebx, 0

    mov dword [res],0  ;float res = 0
innerLoop: ;j = ebx
    cmp ebx, [filter_length]
    jge innerLoopDone

    mov esi,ebx
    mov eax,[filter_length]
    shr eax,1
    sub esi,eax  ;esi = real = j-filter_length         
    add esi,ecx  ;esi = real + i
    cmp esi,0    
    jge iflable  ;if real + i>=0
    cmp esi,[input_length]
    jl iflable   ;if real + i < input_lenght

    inc ebx
    jmp innerLoop

innerLoopDone:
    mov edi,0

    xorps xmm2,xmm2

    fld dword [res]
    mov eax,[output]
    fst dword [eax+ecx*4] ;output[i] = res

    inc ecx
    jmp outerLoop

iflable: 
    mov edi,esi

    mov eax,[input]
    movaps xmm0,[eax + edi*4]
    
    mov eax,[filter]
    movaps xmm1,[eax + ebx*4]
    
    mulps xmm0,xmm1

    call SSEHorizontalSum32
    
    addps xmm2,xmm0
    movaps [res],xmm2
    
    add edi, 4

    ;fld [input+esi*4]   ;input[i+real]
    ;fmul [filter+4*ebx] ;input[i+real]*filter[j]
    ;fadd [res]
    ;fst [res]           ;res += input[i+real]*filter[j]

    ;cmp ebx, [filter_length]
    ;jge innerLoopDone
    add ebx,4
    jmp innerLoop
done:
    ret

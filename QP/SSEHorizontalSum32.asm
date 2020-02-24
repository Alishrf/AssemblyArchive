section .data 
align 16
buffer: dd 0, 0, 0, 0
align 16
output: dd 0,0,0,0

section .text

global SSEHorizontalSum32

;inputs must be 4 32-bit floating points placed in xmm0
;output will be in xmm0 as a single floating point

SSEHorizontalSum32:
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

    fst dword [output]
    movaps xmm0, [output]

    ;clear all buffered data
    ;for security issues
    mov dword [output], 0
    mov dword [buffer], 0
    mov dword [buffer+4], 0
    mov dword [buffer+8], 0
    mov dword [buffer+12], 0
    
    ret

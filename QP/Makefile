GCC_OPTIONS= -m32
NASM_OPTIONS= -f elf

first.out: main.o asm_io.o a.o
	gcc $(GCC_OPTIONS) -o first main.o asm_io.o a.o SSEHorizontalSum32.o


main.o: main.c
	gcc $(GCC_OPTIONS) -c main.c
	
a.o: a.asm
	nasm $(NASM_OPTIONS) a.asm

SSEHorizontalSum32.o: SSEHorizontalSum32.asm
	nasm $(NASM_OPTIONS) SSEHorizontalSum32.asm

asm_io.o: asm_io.asm
	nasm $(NASM_OPTIONS) -d ELF_TYPE asm_io.asm


GCC_OPTIONS= -m32
NASM_OPTIONS= -f elf

first.out: driver.o main.o io.o merge.o asm_io.o
	gcc $(GCC_OPTIONS) -o first driver.o main.o io.o merge.o asm_io.o


main.o: main.asm 
	nasm $(NASM_OPTIONS) main.asm


io.o: io.asm
	nasm $(NASM_OPTIONS) io.asm

merge.o: merge.asm
	nasm $(NASM_OPTIONS) merge.asm	

asm_io.o: asm_io.asm
	nasm $(NASM_OPTIONS) -d ELF_TYPE asm_io.asm

driver.o: driver.c
	gcc $(GCC_OPTIONS) -c driver.c

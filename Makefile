all:test asm main

main:
	gcc -o main main.c
asm:
	as -o asm.o asm.s
	ld -s -o asm asm.o
test:
	gcc -o test test.c
clean:
	rm test asm main out

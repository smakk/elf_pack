all:test asm main par

main:
	gcc -o main main.c
asm:
	as -o asm.o asm.s
	ld -s -o asm asm.o
test:
	gcc -o test test.c
par:
	gcc -nostdlib -nostartfiles -fPIC -pie -fomit-frame-pointer par.c -o par
clean:
	rm test asm main out par

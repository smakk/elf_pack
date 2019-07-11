.text
.global _start
_start:
	movq $0x0000000000000001,%rax
	movq $0x1000000000000001,%rbx
.loop:
	addq $0x00000001, (%rax)
	addq $0x8, %rax
	sub $1,%rbx
	cmp $0,%rbx
	jne .loop
	jmp 0x00000001
	movq $0,%rdi
	movq $60,%rax
	syscall

.text
.global _start
_start:
	jmp 0x0000000000000001
	movq $0,%rdi
	movq $60,%rax
	syscall

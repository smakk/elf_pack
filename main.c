#include <elf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	//读取test文件
	int fd = open("./test", O_RDWR);
	if(fd<0){
		printf("open test fail\n");
	}
	struct stat old_st;
	fstat(fd, &old_st);
	void * old_addr = mmap(0, old_st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	//读取asm文件
	struct stat st;
	int asm_file = open("./par", O_RDWR);
	fstat(asm_file, &st);
	void * asm_addr = mmap(0, st.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, asm_file, 0);
	unsigned long asm_size = ((Elf64_Shdr*)(((Elf64_Ehdr*)asm_addr)->e_shoff + asm_addr)+6)->sh_size;
	unsigned long asm_code_offset = ((Elf64_Shdr*)(((Elf64_Ehdr*)asm_addr)->e_shoff + asm_addr)+6)->sh_offset;
	void* out_mem = malloc(old_st.st_size+asm_size);
	memcpy(out_mem, old_addr, old_st.st_size);
	
	//1、更改头elf头中的节区表位置
	Elf64_Ehdr* ehdr = out_mem;
	ehdr->e_shoff += asm_size;
	
	//2、修改data段的大小信息，和权限
	Elf64_Phdr* text_phdr = NULL;
	Elf64_Phdr* data_phdr = NULL;
	Elf64_Phdr* current = out_mem + ehdr->e_phoff;
	int i;
	for(i=0;i<ehdr->e_phnum;i++){
		if(current->p_type == PT_LOAD){
			current->p_flags = current->p_flags | PF_X | PF_R | PF_W;
			if(text_phdr == NULL) text_phdr = current;
			else data_phdr = current;
		}
		current++;
	}
	//入口地址个data段大小
	unsigned long old_entry = ehdr->e_entry;
	ehdr->e_entry = data_phdr->p_vaddr + data_phdr->p_filesz;
	//printf("%lx\n", ehdr->e_entry);
	data_phdr->p_filesz += asm_size;
	data_phdr->p_memsz += asm_size;
	
	//3、修改.bss节头,bss应该是最后一个被加载进内存的节头,bss末尾应该是data段的末尾
	//4、对bss节之后的所有节的地址进行调整
	Elf64_Shdr* curr = out_mem + ehdr->e_shoff-asm_size;
	Elf64_Shdr* shstr = curr+ehdr->e_shstrndx;
	Elf64_Shdr* text_pos = NULL;
	Elf64_Shdr* bss_pos = NULL;
	int kk;
	for(i=0;i<ehdr->e_shnum;i++){
		//这里因为p_memsz已经被修改了，所以需要减去asm_size
		//printf("%s\n", (char*)(out_mem+shstr->sh_offset+curr->sh_name));
		if(strcmp(".text",(char*)(out_mem+shstr->sh_offset+curr->sh_name)) == 0){
			text_pos = curr;
		}
		if(curr->sh_addr+curr->sh_size == data_phdr->p_vaddr+data_phdr->p_memsz-asm_size){
			bss_pos = (void*)curr;
			kk = 1;
			//这里没有continue，bss节的offset也是需要调整的
		}
		if(kk == 1){
			curr->sh_offset += asm_size;
		}
		curr++;
	}
	//memmove(out_mem+bss_pos->sh_offset, out_mem+bss_pos->sh_offset-asm_size, old_st.st_size-bss_pos->sh_offset);
	//memcpy(out_mem+bss_pos->sh_offset-asm_size, asm_addr+asm_code_offset, asm_size);
	
	int out_file = open("./out", O_RDWR | O_CREAT);
	/*
	//测试
	Elf64_Ehdr* ehdr = out_mem;
	ehdr->e_entry = 0x0000000000601040;
	int i;
	Elf64_Phdr* current = out_mem + ehdr->e_phoff;
	Elf64_Phdr* text_phdr = NULL;
	Elf64_Phdr* data_phdr = NULL;
	for(i=0;i<ehdr->e_phnum;i++){
		if(current->p_type == PT_LOAD){
			current->p_flags = current->p_flags | PF_X | PF_R | PF_W;
			if(text_phdr == NULL) text_phdr = current;
			else data_phdr = current;
		}
		current++;
	}
	data_phdr->p_filesz += asm_size;
	data_phdr->p_memsz += asm_size;
	
	write(out_file, out_mem, 0x1040);
	write(out_file, asm_addr+asm_code_offset, asm_size);
	write(out_file, out_mem+0x1040, old_st.st_size-0x1040);
	*/
	//write(out_file, out_mem, old_st.st_size+asm_size);
	//先写入bss节前面的所有部分，然后是汇编代码，最后是文件的后一部分,由于前面已经将bss的大小进行了调整，所以这里也需要调整一下
	unsigned long code_size = text_pos->sh_size/sizeof(unsigned long);
	//这里是向par的代码中填充一些关键数据，地址需要根据汇编代码算出来，入口地址根据jmp指令的位置算出，另外两个值分别填充进nop指令处
	//入口地址调整
	unsigned int *tar = asm_addr+asm_code_offset+0x98;
	*tar = old_entry - (data_phdr->p_vaddr + data_phdr->p_filesz-asm_size+0x9c);
	//代码段开始位置
	unsigned long* tt = asm_addr+asm_code_offset+0x9d;
	*tt = text_pos->sh_addr;
	//代码段大小
	tt = asm_addr+asm_code_offset+0xa5;
	*tt = code_size;
	unsigned long * code_begin = out_mem+text_pos->sh_offset;
	
	//printf("%lx\n", *(unsigned long*)(asm_addr+asm_code_offset));
	int j;
	
	for(j=0;j<code_size;j++){
		*code_begin = ~(*code_begin);
		code_begin++;
	}
	
	write(out_file, out_mem, ((Elf64_Shdr*)bss_pos)->sh_offset-asm_size);
	write(out_file, asm_addr+asm_code_offset, asm_size);
	write(out_file, out_mem+((Elf64_Shdr*)bss_pos)->sh_offset-asm_size, old_st.st_size-((Elf64_Shdr*)bss_pos)->sh_offset+asm_size);
	fchmod(out_file, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH);
	close(out_file);
	close(asm_file);
	close(fd);
}

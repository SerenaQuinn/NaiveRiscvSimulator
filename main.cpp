#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <elf.h>
#include <fcntl.h> 
#include <unordered_map>
#include "decode.cpp"
using namespace std;

const int memory_size = 256 * 1024 * 1024;
long long reg[64];
float freg[32];
char memory[memory_size];
unsigned long long PC;

int main (int argc, char **argv)
{
	if (argc != 2) {
		printf ("need file name\n");
	}

	int fd = open (argv[1], O_RDONLY, 0);
	char ehdrbuffer[1024];
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)ehdrbuffer;
	read (fd, ehdrbuffer, sizeof(Elf64_Ehdr));

	char *buffer = new char[ehdr->e_phnum * sizeof(Elf64_Phdr)];
	Elf64_Phdr *ph = (Elf64_Phdr *) buffer;
	Elf64_Phdr *eph = ph + ehdr->e_phnum;
	lseek (fd, ehdr->e_phoff, SEEK_SET);
	read (fd, buffer, ehdr->e_phnum * sizeof(Elf64_Phdr));

	for (; ph < eph; ph++) {
		if (ph->p_type == PT_LOAD) {
			lseek (fd, ph->p_offset, SEEK_SET);
			read (fd, &memory[ph->p_vaddr], ph->p_filesz);
		}
	}

	close (fd);

	PC = ehdr->e_entry;
	reg[2] = memory_size - 0xF0; // initiate the sp pointer
	while (true) {
		unsigned ins = *(unsigned *)(memory + PC);
		// printf("PC:0x%llx\n", PC);
		PC += 4;
		decode (ins);
		// printf("reg[0]:%lld\nreg[1]:%lld\n", reg[0], reg[1]);
		// for(int i = 3; i < 32; i++){
		// 	printf("reg[%d]:%lld\n", i, reg[i]);
		// }
		// if(PC >= 0x14478 && PC <= 0x144c4){
		// 	printf("sp:%u\n", *(unsigned*)(memory+reg[2]+16));
		// }
	}
}
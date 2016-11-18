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
// unsigned long long end = 0;

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
			// end = max(end, ph->p_vaddr + ph->p_memsz);
			// printf ("vaddr: %lx\nfilesize: %ld\n", ph->p_vaddr, ph->p_filesz);
		}
	}

	close (fd);

	PC = ehdr->e_entry;
	reg[2] = memory_size; // initiate the sp pointer
	while (true) {
		unsigned ins = *(unsigned *)(memory + PC);
		// printf("PC = %llx\n", PC);
		PC += 4;
		decode (ins);
		// printf("a0 = %lld\n", reg[10]);
	}
}
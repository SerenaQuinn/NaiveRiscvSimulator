#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <elf.h>
#include <unistd.h>
#include <fcntl.h> 
#include <unordered_map>
#include "decode.cpp"
using namespace std;

// class Memory {
// 	unordered_map<unsigned long long, char> m;
// 	char read_char_memory (unsigned long long addr) {
// 		return m[addr];
// 	}
// 	void write_char_memory (unsigned long long addr, char ch) {
// 		m[addr] = ch;
// 	}
// 	int read_int_memory (unsigned long long addr) {
// 		int ret = 0;
// 		for (int i = 3; i >= 0; i--) {
// 			ret = (ret << 8) | m[addr + i];
// 		}
// 		return ret;
// 	}
// }

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
			// printf ("vaddr: %lx\nfilesize: %ld\n", ph->p_vaddr, ph->p_filesz);
		}
	}

	close (fd);

	PC = ehdr->e_entry;
	reg[2] = 0x800000;
	while (true) {
		unsigned ins = *(unsigned *)(memory + PC);
		// printf ("PC = %llx\tinstruction = %x\n", PC, ins);
		PC += 4;
		decode (ins);
	}
}
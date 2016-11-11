typedef unsigned long long ull;
typedef long long ll;

extern ll reg[64];
extern float freg[32];
extern char memory[];
extern ull PC;

void LUI (unsigned rd, int imm) {
	reg[rd] = (ll)imm;
}

void AUIPC (unsigned rd, int imm) {
	reg[rd] = PC - 4 + imm;
}

void JAL (unsigned rd, int imm) {
	if (rd != 0) {
		reg[rd] = PC;
	}
	PC = PC - 4;
	PC += (int)imm;
}

void JALR (unsigned rd, unsigned rs1, int imm) {
	if (rd != 0) {
		reg[rd] = PC;
	}
	PC = ((reg[rs1] + imm) >> 1) << 1;
}

void BEQ (unsigned rs1, unsigned rs2, int imm) {
	if (reg[rs1] == reg[rs2]) {
		PC = PC - 4 + imm;
	}
}

void BNE (unsigned rs1, unsigned rs2, int imm) {
	if (reg[rs1] != reg[rs2]) {
		PC = PC - 4 + imm;
	}
}

void BLT (unsigned rs1, unsigned rs2, int imm) {
	if (reg[rs1] < reg[rs2]) {
		PC = PC - 4 + imm;
	}
}

void BGE (unsigned rs1, unsigned rs2, int imm) {
	if (reg[rs1] >= reg[rs2]) {
		PC = PC - 4 + imm;
	}
}

void BLTU (unsigned rs1, unsigned rs2, int imm) {
	if ((ull)reg[rs1] < (ull)reg[rs2]) {
		PC = PC - 4 + imm;
	}
}

void BGEU (unsigned rs1, unsigned rs2, int imm) {
	if ((ull)reg[rs1] >= (ull)reg[rs2]) {
		PC = PC - 4 + imm;
	}
}

/////////////////////////////////////////////////////////
void LB (unsigned rd, unsigned rs1, int imm) {
	// load 8 bits
	ull addr = reg[rs1] + imm;
	reg[rd] = (ll)(memory[addr]);
}

void LBU (unsigned rd, unsigned rs1, int imm) {
	// load 8 bits 0 extend
	ull addr = reg[rs1] + imm;
	reg[rd] = (ll)(ull)(memory[addr]);
}

void LH (unsigned rd, unsigned rs1, int imm) {
	ull addr = reg[rs1] + imm;
	short *p = (short *)(memory + addr);
	reg[rd] = (ll)(*p);
}

void LHU (unsigned rd, unsigned rs1, int imm) {
	ull addr = reg[rs1] + imm;
	unsigned short* p = (unsigned short*)(memory + addr);
	reg[rd] = (ll)(ull)(*p);
}

void LW (unsigned rd, unsigned rs1, int imm) {
	ull addr = reg[rs1] + imm;
	reg[rd] = (ll)(*(int *)(memory + addr));
}

void LWU (unsigned rd, unsigned rs1, int imm) {
	ull addr = reg[rs1] + imm;
	reg[rd] = (ll)(*(unsigned *)(memory + addr));
}

void LD (unsigned rd, unsigned rs1, int imm) {
	ull addr = reg[rs1] + imm;
	reg[rd] = (ll)(*(ll *)(memory + addr));
}

void SB (unsigned rs1, unsigned rs2, int imm) {
	ull addr = reg[rs1] + imm;
	memory[addr] = (char)reg[rs2];
}

void SH (unsigned rs1, unsigned rs2, int imm) {
	ull addr = reg[rs1] + imm;
	*(unsigned short*)(memory + addr) = (unsigned short)reg[rs2];
}

void SW (unsigned rs1, unsigned rs2, int imm) {
	ull addr = reg[rs1] + imm;
	*(int *)(memory + addr) = (int)reg[rs2];
}

void SD (unsigned rs1, unsigned rs2, int imm) {
	ull addr = reg[rs1] + imm;
	*(ll*)(memory + addr) = (ll)reg[rs2];
}

void ADD (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = reg[rs1] + reg[rs2];
}

void ADDW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)(int)(reg[rs1] + reg[rs2]);
}

void ADDI (unsigned rd, unsigned rs1, int imm) {
	reg[rd] = reg[rs1] + (ll)imm;
}

void ADDIW (unsigned rd, unsigned rs1, int imm) {
	reg[rd] = (ll)(int)(reg[rs1] + imm);
}

void SUB (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = reg[rs1] - reg[rs2];
}

void SUBW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)(int)(reg[rs1] - reg[rs2]);
}

void SLTI (unsigned rd, unsigned rs1, int imm) {
	reg[rd] = (reg[rs1] < (ll)imm) ? 1 : 0;
	// if (reg[rs1] < (ll)imm)
	// 	reg[rd] = 1;
	// else
	// 	reg[rd] = 0;
}

void SLTIU (unsigned rd, unsigned rs1, int imm) {
	reg[rd] = ((ull)reg[rs1] < (ull)imm) ? 1 : 0;
	// if ((ull)reg[rs1] < (ull)imm)
	// 	reg[rd] = 1;
	// else
	// 	reg[rd] = 0;
}

void XORI (unsigned rd, unsigned rs1, int imm) {
	reg[rd] = reg[rs1] ^ (ll)imm;
}

void ORI (unsigned rd, unsigned rs1, int imm) {
	reg[rd] = reg[rs1] | (ll)imm;
}

void ANDI (unsigned rd, unsigned rs1, int imm) {
	reg[rd] = reg[rs1] & (ll)imm;
}

void AND (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] =reg[rs1] & reg[rs2];
}

void SLLI (unsigned rd, unsigned rs1, unsigned shamt) {
	reg[rd] = reg[rs1] << shamt;
}

void SRLI (unsigned rd, unsigned rs1, unsigned shamt) {
	reg[rd] = (ull)reg[rs1] >> shamt;
}

void SRAI (unsigned rd, unsigned rs1, unsigned shamt) {
	reg[rd] = reg[rs1] >> shamt;
}

void SLL (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ull)reg[rs1] << (reg[rs2] & ((1 << 5) - 1));
}

void SLT (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (reg[rs1] < reg[rs2]) ? 1 : 0;
	// if (reg[rs1] < reg[rs2])
	// 	reg[rd] = 1;
	// else
	// 	reg[rd] = 0;
}

void SLTU (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = ((ull)reg[rs1] < (ull)reg[rs2]) ? 1 : 0;
	// if ((ull)reg[rs1] < (ull)reg[rs2])
	// 	reg[rd] = 1;
	// else
	// 	reg[rd] = 0;
}

void XOR (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = reg[rs1] ^ reg[rs2];
}

void SRL (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ull)reg[rs1] >> (reg[rs2] & ((1 << 5) - 1));
}

void SRA (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = reg[rs1] >> (reg[rs2] & ((1 << 5) - 1));
}

void OR (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = reg[rs1] | reg[rs2];
}

void SLLIW (unsigned rd, unsigned rs1, unsigned shamt) {
	reg[rd] = (ll)((int)reg[rs1] << shamt);
}

void SRLIW (unsigned rd, unsigned rs1, unsigned shamt) {
	reg[rd] = (ll)((unsigned)reg[rs1] >> shamt);
}

void SRAIW (unsigned rd, unsigned rs1, unsigned shamt) {
	reg[rd] = (ll)((int)reg[rs1] >> shamt);
}

void SLLW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)((int)reg[rs1] << (reg[rs2] & 31));
}

void SRLW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)((unsigned)reg[rs1] >> (reg[rs2] & 31));
}

void SRAW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)((int)reg[rs1] >> (reg[rs2] & 31));
}

//////////////////////////////FFFFFFFFFFF
void FLW (unsigned rd, unsigned rs1, int imm) {
	ull addr = reg[rs1] + imm;
	freg[rd] = *(float *)(memory + addr);
}

void FSW (unsigned rs1, unsigned rs2, int imm) {
	ull addr = reg[rs1] + imm;
	*(float *)(memory + addr) = freg[rs2];
}

void FMADDS (unsigned rd, unsigned rs1, unsigned rs2, unsigned rs3) {
	freg[rd] = freg[rs1] * freg[rs2] + freg[rs3];
}

void FMSUBS (unsigned rd, unsigned rs1, unsigned rs2, unsigned rs3) {
	freg[rd] = freg[rs1] * freg[rs2] - freg[rs3];
}

void FNMADDS (unsigned rd, unsigned rs1, unsigned rs2, unsigned rs3) {
	freg[rd] = -(freg[rs1] * freg[rs2] + freg[rs3]);
}

void FNMSUBS (unsigned rd, unsigned rs1, unsigned rs2, unsigned rs3) {
	freg[rd] = -(freg[rs1] * freg[rs2] - freg[rs3]);
}

void FADDS (unsigned rd, unsigned rs1, unsigned rs2) {
	freg[rd] = (float)(freg[rs1] + freg[rs2]);
}

void FSUBS (unsigned rd, unsigned rs1, unsigned rs2) {
	freg[rd] = (float)(freg[rs1] - freg[rs2]);
}

void FMULS (unsigned rd, unsigned rs1, unsigned rs2) {
	freg[rd] = (float)(freg[rs1] * freg[rs2]);
}

void FDIVS (unsigned rd, unsigned rs1, unsigned rs2) {
	freg[rd] = (float)(freg[rs1] / freg[rs2]);
}

void FCVTSW (unsigned rd, unsigned rs1) {
	freg[rd] = (float)(int)reg[rs1];
}

void FCVTSWU (unsigned rd, unsigned rs1) {
	freg[rd] = (float)(unsigned)reg[rs1];
}

void FCVTSL (unsigned rd, unsigned rs1) {
	freg[rd] = (float)(reg[rs1]);
}

void FCVTSLU (unsigned rd, unsigned rs1) {
	freg[rd] = (float)(ull)reg[rs1];
}

// *************************************************************************************

void MUL (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (reg[rs1] * reg[rs2]) & 0xFFFFFFFF;
}

void MULH (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = ((reg[rs1] * reg[rs2]) >> 32) & 0xFFFFFFFF;
}

void MULHSU (unsigned rd, unsigned rs1, unsigned rs2) {
	ull temp = reg[rs1] * (ull)reg[rs2];
	reg[rd] = (temp >> 32) & 0xFFFFFFFF;
}

void MULHU (unsigned rd, unsigned rs1, unsigned rs2) {
	ull temp = (ull)reg[rs1] * (ull)reg[rs2];
	reg[rd] = (temp >> 32) & 0xFFFFFFFF;
}

void DIV (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = reg[rs1] / reg[rs2];
}

void DIVU (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ull)reg[rs1] / (ull)reg[rs2];
}

void REM (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = reg[rs1] % reg[rs2];
}

void REMU (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ull)reg[rs1] % (ull)reg[rs2];
}

void MULW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)((int)(reg[rs1] * reg[rs2]));
}

void DIVW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)((int)reg[rs1] / (int)reg[rs2]);
}

void DIVUW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)((unsigned)reg[rs1] / (unsigned)reg[rs2]);
}

void REMW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)((int)reg[rs1] % (int)reg[rs2]);
}

void REMUW (unsigned rd, unsigned rs1, unsigned rs2) {
	reg[rd] = (ll)((unsigned)reg[rs1] % (unsigned)reg[rs2]);
}

void ECALL () {
	switch (reg[17]) {
		case 57:
			break;
		case 62:
			break;
		case 63:
			break;
		case 64:
			printf ("a0 = %lld\na1 = %lld\na2 = %lld\n", reg[10], reg[11], reg[12]);
			write (reg[10], (void *)reg[11], (int)reg[12]);
			break;
		case 93:
			exit (0);
			break;
		// case 169:
		// 	break;
		// case 214:
		// 	break;
		default:
			printf ("system call %lld not implemented\n", reg[17]);
			exit (0);
	}
}
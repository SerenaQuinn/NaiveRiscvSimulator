#include "execute.cpp"

// [start, end]
unsigned get_section (unsigned a, int start, int end) {
	return (a  >> start) & ((-1u >> (31 + start - end)));
}

unsigned instr;
unsigned opcode, rd, funct3, rs1, rs2, rs3, funct2, funct7;

int I_type () {
	int imm = get_section (instr, 20, 30);
	imm |= ((int)get_section (instr, 31, 31) << 31) >> 20;
	return imm;
}

int S_type () {
	int imm = get_section (instr, 7, 11);
	imm |= get_section (instr, 25, 30) << 5;
	imm |= ((int)get_section (instr, 31, 31) << 31) >> 20;
	return imm;
}

int SB_type () {
	int imm = get_section (instr, 8, 11) << 1;
	imm |= get_section (instr, 25, 30) << 5;
	imm |= get_section (instr, 7, 7) << 11;
	imm |= ((int)get_section (instr, 31, 31) << 31) >> 19;
	return imm;
}

int U_type () {
	int imm = get_section (instr, 12, 31) << 12;
	return imm;
}

int UJ_type () {
	int imm = get_section (instr, 21, 30) << 1;
	imm |= get_section (instr, 20, 20) << 11;
	imm |= get_section (instr, 12, 19) << 12;
	imm |= ((int)get_section (instr, 31, 31) << 31) >> 11;
	return imm;
}

void panic() {
	printf ("PC = %llx\ninstruction = %x\n", PC, instr);
	exit (0);
}

void branch (int);
void load (int);
void store (int);
void alui (int);
void alu ();
void csrr ();
void aluiw (int);
void aluw ();
void falus ();

void decode (unsigned ins) {
	instr = ins;
	opcode = get_section (instr, 0, 6);
	rd = get_section (instr, 7, 11);
	rs1 = get_section (instr, 15, 19);
	rs2 = get_section (instr, 20, 24);
	rs3 = get_section (instr, 27, 31);
	funct2 = get_section (instr, 25, 26);
	funct3 = get_section (instr, 12, 14);
	funct7 = get_section (instr, 25, 31);
	
	switch (opcode) {
		case 0b0110111:
			LUI (rd, U_type ());
			break;
		case 0b0010111:
			AUIPC (rd, U_type ());
			break;
		case 0b1101111:
			JAL (rd, UJ_type ());
			break;
		case 0b1100111:
			JALR (rd, rs1, I_type ());
			break;
		case 0b1100011:
			branch (SB_type ());
			break;
		case 0b0000011:
			load (I_type ());
			break;
		case 0b0100011:
			store (S_type ());
			break;
		case 0b0010011:
			alui (I_type ());
			break;
		case 0b0110011:
			alu ();
			break;
		case 0b0001111:
			printf ("FENCE not implemented\n");
			panic ();
		case 0b1110011:
			csrr ();
			break;
		case 0b0011011:
			aluiw (I_type ());
			break;
		case 0b0111011:
			aluw ();
			break;
		case 0b0000111:
			FLW (rd, rs1, I_type ());
			break;
		case 0b0100111:
			FSW (rs1, rs2, S_type ());
			break;
		case 0b1000011:
			FMADDS (rd, rs1, rs2, rs3);
			break;
		case 0b1000111:
			FMSUBS (rd, rs1, rs2, rs3);
			break;
		case 0b1001011:
			FNMSUBS (rd, rs1, rs2, rs3);
			break;
		case 0b1001111:
			FNMADDS (rd, rs1, rs2, rs3);
			break;
		case 0b1010011:
			falus ();
			break;
		default: 
			printf ("unknown opcode %d\n", opcode);
			panic ();
	}
}

void branch (int imm) {
	switch (funct3) {
		case 0:
			BEQ (rs1, rs2, imm);
			break;
		case 1:
			BNE (rs1, rs2, imm);
			break;
		case 4:
			BLT (rs1, rs2, imm);
			break;
		case 5:
			BGE (rs1, rs2, imm);
			break;
		case 6:
			BLTU (rs1, rs2, imm);
			break;
		case 7:
			BGEU (rs1, rs2, imm);
			break;
		default:
			printf ("branch error funct3!\n");
			panic ();
	}
}

void load (int imm) {
	switch (funct3) {
		case 0:
			LB (rd, rs1, imm);
			break;
		case 1:
			LH (rd, rs1, imm);
			break;
		case 2:
			LW (rd, rs1, imm);
			break;
		case 3:
			LD (rd, rs1, imm);
			break;
		case 4:
			LBU (rd, rs1, imm);
			break;
		case 5:
			LHU (rd, rs1, imm);
			break;
		case 6:
			LWU (rd, rs1, imm);
		default:
			printf ("load funct3 error: %d\n", funct3);
			exit (0);
	}
}

void store (int imm) {
	switch (funct3) {
		case 0:
			SB (rs1, rs2, imm);
			break;
		case 1:
			SH (rs1, rs2, imm);
			break;
		case 2:
			SW (rs1, rs2, imm);
			break;
		case 3:
			SD (rs1, rs2, imm);
			break;
		default:
			printf ("store funct3 error: %d\n", funct3);
			panic ();
	}
}

void alui (int imm) {
	switch (funct3) {
		case 0:
			ADDI (rd, rs1, imm);
			break;
		case 1:
			SLLI (rd, rs1, rs2);
			break;
		case 2:
			SLTI (rd, rs1, imm);
			break;
		case 3:
			SLTIU (rd, rs1, imm);
			break;
		case 4:
			XORI (rd, rs1, imm);
			break;
		case 5:
			if (funct7) {
				SRAI (rd, rs1, rs2);
			}
			else {
				SRLI (rd, rs1, rs2);
			}
			break;
		case 6:
			ORI (rd, rs1, imm);
			break;
		case 7:
			ANDI (rd, rs1, imm);
			break;
		default:
			printf ("can never go to here\n");
			panic ();
	}
}

void alu () {
	if (funct7 == 1) {
		switch (funct3) {
			case 0:
				MUL (rd, rs1, rs2);
				break;
			case 1:
				MULH (rd, rs1, rs2);
				break;
			case 2:
				MULHSU (rd, rs1, rs2);
				break;
			case 3:
				MULHU (rd, rs1, rs2);
				break;
			case 4:
				DIV (rd, rs1, rs2);
				break;
			case 5:
				DIVU (rd, rs1, rs2);
				break;
			case 6:
				REM (rd, rs1, rs2);
				break;
			case 7:
				REMU (rd, rs1, rs2);
				break;
			default:
				panic ();
		}
	}
	else {
		switch (funct3) {
			case 0:
				if (funct7) {
					SUB (rd, rs1, rs2);
				}
				else {
					ADD (rd, rs1, rs2);
				}
				break;
			case 1:
				SLL (rd, rs1, rs2);
				break;
			case 2:
				SLT (rd, rs1, rs2);
				break;
			case 3:
				SLTU (rd, rs1, rs2);
				break;
			case 4:
				XOR (rd, rs1, rs2);
				break;
			case 5:
				if (funct7) {
					SRA (rd, rs1, rs2);
				}
				else {
					SRL (rd, rs1, rs2);
				}
				break;
			case 6:
				OR (rd, rs1, rs2);
				break;
			case 7:
				ADD (rd, rs1, rs2);
				break;
			default:
				printf ("can never go to here\n");
				panic ();
		}
	}
}

void csrr () {
	if (instr == 0x73) {
		ECALL ();
	}
	else {
		printf ("csrr not implemented\n");
		panic ();
	}
}

void aluiw (int imm) {
	switch (funct3) {
		case 0:
			ADDIW (rd, rs1, imm);
			break;
		case 1:
			SLLIW (rd, rs1, rs2);
			break;
		case 5:
			if (funct7) {
				SRAIW (rd, rs1, rs2);
			}
			else {
				SRLIW (rd, rs1, rs2);
			}
			break;
		default:
			printf ("aluiw funct3 error: %d\n", funct3);
			panic ();
	}
}

void aluw () {
	switch (funct3) {
		case 0:
			if (funct7 == 1) {
				MULW (rd, rs1, rs2);
			}
			else if (funct7) {
				SUBW (rd, rs1, rs2);
			}
			else {
				ADDW (rd, rs1, rs2);
			}
			break;
		case 1:
			SLLW (rd, rs1, rs2);
			break;
		case 4:
			DIVW (rd, rs1, rs2);
		case 5:
			if (funct7 == 1) {
				DIVUW (rd, rs1, rs2);
			}
			else if (funct7) {
				SRAW (rd, rs1, rs2);
			}
			else {
				SRLW (rd, rs1, rs2);
			}
			break;
		case 6:
			REMW (rd, rs1, rs2);
			break;
		case 7:
			REMUW (rd, rs1, rs2);
		default:
			printf ("aluw funct3 error: %d\n", funct3);
			panic ();
	}
}

void falus () {
	switch (funct7) {
		case 0b0000000:
			FADDS (rd, rs1, rs2);
			break;
		case 0b0000100:
			FSUBS (rd, rs1, rs2);
			break;
		case 0b0001000:
			FMULS (rd, rs1, rs2);
			break;
		case 0b0001100:
			FDIVS (rd, rs1, rs2);
			break;
		// case 0b0101100:
		// 	FSQRTS (rd, rs1);
		// 	break;
		// case 0b0010000:
		// 	switch (funct3) {
		// 		case 0:
		// 			FSGNJS (rd, rs1, rs2);
		// 			break;
		// 		case 1:
		// 			FSGNJNS (rd, rs1, rs2);
		// 			break;
		// 		case 2:
		// 			FSGNJXS (rd, rs1, rs2);
		// 			break;
		// 		default:
		// 			printf ("fsgn funct3 error: %d\n", funct3);
		// 			panic ();
		// 	}
		// 	break;
		// case 0b0010100:
		// 	if (funct3) {
		// 		FMAXS (rd, rs1, rs2);
		// 	}
		// 	else {
		// 		FMINS (rd, rs1, rs2);
		// 	}
		// 	break;
		// case 0b1100000:
		// 	switch (rs2) {
		// 		case 0:
		// 			FCVTWS (rd, funct3, rs1);
		// 			break;
		// 		case 1:
		// 			FCVTWUS (rd, funct3, rs1);
		// 			break;
		// 		case 2:
		// 			FCVTLS (rd, funct3, rs1);
		// 			break;
		// 		case 3:
		// 			FCVTLUS (rd, funct3, rs1);
		// 		default:
		// 			printf ("fcvt rs2 error: %d\n", rs2);
		// 			panic ();
		// 	}
		// 	break;
		// case 0b1010000:
		// 	if (funct3 == 0) {
		// 		FLES (rd, rs1, rs2);
		// 	}
		// 	else if (funct3 == 1) {
		// 		FLTS (rd, rs1, rs2);
		// 	}
		// 	else {
		// 		FEQS (rd, rs1, rs2);
		// 	}
		// 	break;
		// case 0b1110000:
		// 	if (funct3) {
		// 		FCLASSS (rd, rs1);
		// 	}
		// 	else {
		// 		FMVXS (rd, rs1);
		// 	}
		// 	break;
		case 0b1101000:
			switch (rs2) {
				case 0:
					FCVTSW (rd, rs1);
					break;
				case 1:
					FCVTSWU (rd, rs1);
					break;
				case 2:
					FCVTSL (rd, rs1);
					break;
				case 3:
					FCVTSLU (rd, rs1);
					break;
				default:
					printf ("fcvt rs2 error: %d\n", rs2);
					panic ();
			}
			break;
		// case 0b1111000:
		// 	FMVSX (rd, rs1);
		// 	break;
		default:
			printf ("falus funct7 error: %d\n", funct7);
			panic ();
	}
}
#include "cache.h"

int calc_SignExtImm(int imm)
{
	int tmp;
	int imm_tmp;
	int rst;

	tmp = 1;
	rst = 0;
	imm_tmp = imm;
	while (imm_tmp != 0)
	{
		imm_tmp /= 16;
		tmp *= 16;
	}
	rst = (tmp - imm) * (-1);
	if (imm >= 0x8000)
		return (rst);
	else
		return (imm);
}

int calc_JumpAddr(int addr, int PC)
{
	PC = (PC + 4) & 0xf0000000;
	addr = (addr * 4) & 0x0ffffffc;
	addr = addr + PC;
	return (addr);
}

char format(int OPCODE)
{
	if (OPCODE == 0x0)
		return ('R');
	else if (OPCODE == 0x4 || OPCODE == 0x5 || (OPCODE >= 0x8 && OPCODE <= 0xd) || OPCODE == 0xf || OPCODE == 0x23 || OPCODE == 0x24 || OPCODE == 0x25 || OPCODE == 0x28 || OPCODE == 0x29 || OPCODE == 0x2b || OPCODE == 0x30 || OPCODE == 0x38)
		return ('I');
	else if (OPCODE == 0x2 || OPCODE == 0x3)
		return ('J');
	else
		return (0);
}

void set_debug(char format, int inst, IFID_latch *ifid_input, IDEX_latch *idex_output)
{
	idex_output->debug.PC = ifid_input->PC;
	idex_output->debug.instruction = ifid_input->instruction;
	if (format == 'R')
	{
		idex_output->debug.rs = (inst & 0x03e00000) >> 21;
		idex_output->debug.rt = (inst & 0x001f0000) >> 16;
		idex_output->debug.rd = (inst & 0x0000f800) >> 11;
		idex_output->debug.shamt = (inst & 0x000007c0) >> 6;
		idex_output->debug.funct = (inst & 0x0000003f);
		printf("[Decode] (R-type)\n\tOPCODE: 0x%x\tRS: %d\tRT: %d\tRD: %d\tSHAMT: 0x%x\tFUNCT: 0x%02x\n", idex_output->debug.opcode, idex_output->debug.rs, idex_output->debug.rt, idex_output->debug.rd, idex_output->debug.shamt, idex_output->debug.funct);
	}
	else if (format == 'I')
	{
		idex_output->debug.rs = (inst & 0x03e00000) >> 21;
		idex_output->debug.rt = (inst & 0x001f0000) >> 16;
		idex_output->debug.imm = (inst & 0x0000ffff);
		printf("[Decode] (I-type)\n\tOPCODE: 0x%x\tRS: %d\tRT: %d\tIMM: 0x%x\n", idex_output->debug.opcode, idex_output->debug.rs, idex_output->debug.rt, idex_output->debug.imm);
	}
	else if (format == 'J')
	{
		idex_output->debug.addr = (inst & 0x03ffffff);
		printf("[Decode] (J-type)\n\tOPCODE: 0x%x\tADDR: 0x%08x\n", idex_output->debug.opcode, idex_output->debug.addr);
	}
}

void set_control_signal(int OPCODE, int FUNCT, IDEX_latch *idex_output, REG *R)
{
	if (OPCODE == 0 && (FUNCT != 0x8))
		idex_output->control_signal.RegDst = 1;
	if ((OPCODE != 0) && (OPCODE != 0x4) && (OPCODE != 0x5))
		idex_output->control_signal.ALUSrc = 1;
	if (OPCODE == 0x23)
	{
		idex_output->control_signal.MemtoReg = 1;
		idex_output->control_signal.MemRead = 1;
	}
	if ((OPCODE != 0x2b) && (OPCODE != 0x4) && (OPCODE != 0x5) && (OPCODE != 0x2) && !((OPCODE == 0) && (idex_output->debug.funct == 0x8)))
		idex_output->control_signal.RegWrite = 1;
	if (OPCODE == 0x2b)
		idex_output->control_signal.MemWrite = 1;
	if ((OPCODE == 0x2) || (OPCODE == 0x3))
	{
		idex_output->control_signal.PCSrc1 = 1;
		idex_output->control_signal.Jump = 1;
	}
	if ((OPCODE == 0x4) || (OPCODE == 0x5))
		idex_output->control_signal.Branch = 1;
	// when read invalid register, V1Src or V2Src = 1
	if (R[idex_output->debug.rs].invalid)
		idex_output->control_signal.V1Src = 1;
	if (R[idex_output->debug.rt].invalid && (format(OPCODE) == 'R' || idex_output->control_signal.Branch))
		idex_output->control_signal.V2Src = 1;
}

void set_idex_output(IFID_latch *ifid_input, IDEX_latch *idex_output, REG *R)
{
	idex_output->next_PC = ifid_input->next_PC;
	idex_output->PC4 = ifid_input->PC4;
	idex_output->jump_target = calc_JumpAddr(idex_output->debug.addr, idex_output->debug.PC);
	idex_output->V1 = R[idex_output->debug.rs].value;
	idex_output->sign_imm = calc_SignExtImm(idex_output->debug.imm);
	idex_output->V2 = MUX_ALUSrc(idex_output->control_signal.ALUSrc, idex_output->sign_imm, R[idex_output->debug.rt].value);
	if (idex_output->control_signal.RegWrite)
	{
		if (idex_output->control_signal.PCSrc1)
		{
			idex_output->Rdst = 31;
			idex_output->debug.rt = 31;
		}
		else
			idex_output->Rdst = MUX_REGDst(idex_output->control_signal.RegDst, idex_output->debug.rd, idex_output->debug.rt);
	}
}

void decode(IFID_latch *ifid_input, IFID_latch *ifid_output, IDEX_latch *idex_output, REG *R)
{
	int INSTRUCTION;
	int OPCODE;

	INSTRUCTION = ifid_input->instruction;
	OPCODE = (INSTRUCTION & 0xfc000000) >> 26;
	idex_output->debug.opcode = OPCODE;
	if (!format(OPCODE))
	{
		printf("ERROR: Invalid OPCODE.\n");
		memset(ifid_output, 0, sizeof(IFID_latch));
		memset(idex_output, 0, sizeof(IDEX_latch));
		return;
	}
	set_debug(format(OPCODE), INSTRUCTION, ifid_input, idex_output);
	if (!idex_output->debug.instruction)
	{
		idex_output->next_PC = ifid_input->next_PC;
		return;
	}
	set_control_signal(OPCODE, idex_output->debug.funct, idex_output, R);
	set_idex_output(ifid_input, idex_output, R);
	if (idex_output->control_signal.RegWrite && !idex_output->control_signal.PCSrc1)
		(&R[idex_output->Rdst])->invalid = 1;
	if (idex_output->control_signal.Jump)
		memset(ifid_output, 0, sizeof(IFID_latch));
}

#include "cache.h"

int calc_BranchAddr(int imm)
{
	imm *= 4;
	if (imm >= 0x8000)
		imm = imm | 0xfffc0000;
	return (imm);
}

int ALUOP(IDEX_latch *idex_input)
{
	unsigned int PC;
	int rst;
	int SignExtImm;
	int ZeroExtImm;
	int V1;
	int V2;
	int opcode;
	int funct;
	int shamt;

	PC = idex_input->debug.PC;
	rst = 0;
	SignExtImm = idex_input->sign_imm;
	ZeroExtImm = idex_input->debug.imm;
	V1 = idex_input->V1;
	V2 = idex_input->V2;
	opcode = idex_input->debug.opcode;
	funct = idex_input->debug.funct;
	shamt = idex_input->debug.shamt;
	if (opcode == 0x3)
	{
		rst = PC + 8;
		return (rst);
	}
	if (funct == 0x8)
		rst = V1;
	if (funct == 0x20)
		rst = V1 + V2;
	if (funct == 0x21)
		rst = V1 + V2;
	if (funct == 0x24)
		rst = V1 & V2;
	if (funct == 0x27)
		rst = ~(V1 | V2);
	if (funct == 0x25)
		rst = V1 | V2;
	if (funct == 0x2a)
		rst = (V1 < V2) ? 1 : 0;
	if (funct == 0x2b)
		rst = (V1 < V2) ? 1 : 0;
	if (funct == 0x00)
		rst = V2 << shamt;
	if (funct == 0x02)
		rst = V2 >> shamt;
	if (funct == 0x22)
		rst = V1 - V2;
	if (funct == 0x23)
		rst = V1 - V2;
	if (funct == 0x9)
		rst = PC + 4;
	if (opcode == 0x8)
		rst = V1 + SignExtImm;
	else if (opcode == 0x9)
		rst = V1 + SignExtImm;
	else if (opcode == 0xc)
		rst = V1 & ZeroExtImm;
	else if (opcode == 0xf)
		rst = V2 << 16;
	else if (opcode == 0xd)
		rst = V1 | ZeroExtImm;
	else if (opcode == 0xa)
		rst = (V1 < SignExtImm) ? 1 : 0;
	else if (opcode == 0xb)
		rst = (V1 < SignExtImm) ? 1 : 0;
	else if (opcode == 0x23)
		rst = V1 + SignExtImm;
	else if (opcode == 0x2b)
		rst = V1 + SignExtImm;
	else if (opcode == 0x4)
		rst = (V1 == V2);
	else if (opcode == 0x5)
		rst = (V1 != V2);
	else if (opcode == 0x3)
		rst = PC + 8;
	return (rst);
}

void data_dependency_handle(IDEX_latch *idex_input, IDEX_latch *idex_output, EXMEM_latch *exmem_input, MEMWB_latch *memwb_input, int rst)
{
	int rst1;
	int rst2;

	rst1 = idex_output->V1;
	rst2 = idex_output->V2;
	if (idex_output->control_signal.V1Src)
	{
		if (idex_output->debug.rs == memwb_input->Rdst)
		{
			rst1 = memwb_input->ALU_rst;
			if (memwb_input->control_signal.MemRead)
				rst1 = memwb_input->MEM_value;
		}
		if (idex_output->debug.rs == exmem_input->Rdst)
			rst1 = exmem_input->ALU_rst;
		if (idex_output->debug.rs == idex_input->Rdst)
			rst1 = rst;
		idex_output->V1 = rst1;
	}
	if (idex_output->control_signal.V2Src)
	{
		if (idex_output->debug.rt == memwb_input->Rdst)
			rst2 = memwb_input->ALU_rst;
		if (idex_output->debug.rt == exmem_input->Rdst)
			rst2 = exmem_input->ALU_rst;
		if (idex_output->debug.rt == idex_input->Rdst)
			rst2 = rst;
		idex_output->V2 = rst2;
	}
}

void control_dependency_handle(IFID_latch *ifid_output, IDEX_latch *idex_output, EXMEM_latch *exmem_output, BTB *btb, COUNT *counter)
{
	int PC;
	int bcond;
	int num;
	int cnt;

	num = 0;
	cnt = 0;
	PC = exmem_output->debug.PC;
	bcond = exmem_output->bcond;
	for (int i = 0; i < g_branch_cnt; i++)
	{
		if (btb[i].PC == PC)
		{
			num = i;
			break;
		}
		else
			cnt++;
	}
	if (cnt == g_branch_cnt)
	{
		g_branch_cnt++;
		num = g_branch_cnt;
	}
	// BTB update
	if (bcond)
	{
		(&btb[num])->PC = PC;
		(&btb[num])->target_addr = PC + 4 + exmem_output->jump_target;
	}
	if (btb[num].state_machine == 0)
	{
		// bcond == 0, (Not taken) -> prediction O
		if (!bcond)
		{
			counter->num_predict_O++;
			(&btb[num])->state_machine = 0;
		}
		// bcond == 1, (Taken) -> predcition X
		else
		{
			counter->num_predict_X++;
			(&btb[num])->state_machine = 1;
			// to do branch prediction again
			exmem_output->next_PC = PC;
			exmem_output->prediction_fail = 1;
			memset(ifid_output, 0, sizeof(IFID_latch));
			memset(idex_output, 0, sizeof(IDEX_latch));
		}
		return;
	}
	else if (btb[num].state_machine == 1)
	{
		// Not taken -> prediction O
		if (!bcond)
		{
			counter->num_predict_O++;
			(&btb[num])->state_machine = 0;
		}
		// Taken -> prediction X
		else
		{
			counter->num_predict_X++;
			(&btb[num])->state_machine = 2;
			// to do branch prediction again
			exmem_output->next_PC = PC;
			exmem_output->prediction_fail = 1;
			memset(ifid_output, 0, sizeof(IFID_latch));
			memset(idex_output, 0, sizeof(IDEX_latch));
		}
		return;
	}
	else if (btb[num].state_machine == 2)
	{
		// Not taken -> prediction X
		if (!bcond)
		{
			counter->num_predict_X++;
			(&btb[num])->state_machine = 1;
			// to do branch prediction again
			exmem_output->next_PC = PC;
			exmem_output->prediction_fail = 1;
			memset(ifid_output, 0, sizeof(IFID_latch));
			memset(idex_output, 0, sizeof(IDEX_latch));
		}
		// Taken -> prediction O
		else
		{
			counter->num_predict_O++;
			(&btb[num])->state_machine = 3;
		}
		return;
	}
	else if (btb[num].state_machine == 3)
	{
		// Not taken -> prediction X
		if (!bcond)
		{
			counter->num_predict_X++;
			(&btb[num])->state_machine = 2;
			// to do branch prediction again
			exmem_output->next_PC = PC;
			exmem_output->prediction_fail = 1;
			memset(ifid_output, 0, sizeof(IFID_latch));
			memset(idex_output, 0, sizeof(IDEX_latch));
		}
		// Taken -> prediction O
		else
		{
			counter->num_predict_O++;
			(&btb[num])->state_machine = 3;
		}
		return;
	}
}

void execute(IFID_latch *ifid_output, IDEX_latch *idex_input, IDEX_latch *idex_output, EXMEM_latch *exmem_input, EXMEM_latch *exmem_output, MEMWB_latch *memwb_input, BTB *btb, COUNT *counter)
{
	int rst;
	char fmt;
	if (!(idex_input->debug.instruction))
	{
		printf("[Execute]\t--\n");
		exmem_output->next_PC = idex_input->next_PC;
		return;
	}
	counter->num_executed_inst++;
	rst = ALUOP(idex_input);
	fmt = format(idex_input->debug.opcode);
	exmem_output->debug = idex_input->debug;
	exmem_output->control_signal = idex_input->control_signal;
	data_dependency_handle(idex_input, idex_output, exmem_input, memwb_input, rst);
	if (fmt == 'R')
	{
		counter->num_R_type++;
		// jr stall
		if (idex_input->debug.funct == 0x8)
		{
			exmem_output->jump_target = rst;
			memset(ifid_output, 0, sizeof(IFID_latch));
			memset(idex_output, 0, sizeof(IDEX_latch));
		}
		else
			exmem_output->jump_target = idex_input->PC4;
		exmem_output->ALU_rst = rst;
		exmem_output->Rdst = idex_input->Rdst;
		printf("[Execute]\tRESULT: 0x%x (=%d)\nJ-TARGET: 0x%08x\n", rst, rst, exmem_output->jump_target);
	}
	else if (fmt == 'I')
	{
		counter->num_I_type++;
		if (idex_input->control_signal.Branch)
		{
			exmem_output->jump_target = calc_BranchAddr(idex_input->debug.imm);
			exmem_output->bcond = rst;
			if (exmem_output->bcond)
				counter->num_taken_branch++;
			else
				counter->num_not_taken_branch++;
			control_dependency_handle(ifid_output, idex_output, exmem_output, btb, counter);
			printf("[Execute]\tRESULT: 0x%x (= %d)\nJ-TARGET: 0x%08x\n", rst, rst, exmem_output->jump_target);
		}
		else
		{
			exmem_output->jump_target = idex_input->PC4;
			exmem_output->ALU_rst = rst;
			exmem_output->V2 = idex_input->V2;
			exmem_output->Rdst = idex_input->Rdst;
			printf("[Execute]\tRESULT: 0x%x (= %d)\nJ-TARGET: 0x%08x\n", rst, rst, exmem_output->jump_target);
			// lw stall
			if (idex_input->control_signal.MemRead)
			{
				memset(ifid_output, 0, sizeof(IFID_latch));
				memset(idex_output, 0, sizeof(IDEX_latch));
			}
		}
	}
	else if (fmt == 'J')
	{
		counter->num_J_type++;
		counter->num_jump++;
		exmem_output->jump_target = idex_input->jump_target;
		if (idex_input->debug.opcode == 0x3)
		{
			exmem_output->ALU_rst = rst;
			exmem_output->Rdst = idex_input->Rdst;
		}
		printf("[Execute]\tRESULT: 0x%x (= %d)\nJ-TARGET: 0x%08x\n", rst, rst, exmem_output->jump_target);
	}
}

#include "cache.h"

void fetch(IFID_latch *ifid_input, IFID_latch *ifid_output, IDEX_latch *idex_input, EXMEM_latch *exmem_input, BTB *btb, int *MEM, cacheline *cache, COUNT *counter)
{
	int PC;
	int instruction;
	int i;

	PC = 0;
	i = 0;
	// current PC update
	if (idex_input->control_signal.Jump)
		PC = idex_input->jump_target;
	else if (exmem_input->control_signal.MemRead)
		PC = exmem_input->jump_target;
	else if (exmem_input->prediction_fail)
		PC = exmem_input->next_PC;
	else if (exmem_input->debug.opcode == 0 && exmem_input->debug.funct == 0x8)
		PC = exmem_input->jump_target;
	else
		PC = ifid_input->next_PC;
	if (PC == -1)
		return;
	instruction = MemRead(PC, MEM, cache, counter);
	ifid_output->PC = PC;
	ifid_output->PC4 = PC + 4;
	ifid_output->instruction = instruction;
	// predict next PC
	while (i <= g_branch_cnt)
	{
		if ((PC != 0) && (PC == btb[i].PC))
		{
			if ((btb[i].state_machine == 0) || (btb[i].state_machine == 1))
				ifid_output->next_PC = ifid_output->PC4;
			else if ((btb[i].state_machine == 2) || (btb[i].state_machine == 3))
				ifid_output->next_PC = btb[i].target_addr;
			break;
		}
		else
			ifid_output->next_PC = ifid_output->PC4;
		i++;
	}
	printf("[Fetch]\tPC: 0x%x -> INSTRUCTION: 0x%08x\n", PC, instruction);
}

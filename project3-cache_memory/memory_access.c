#include "cache.h"

void memory_access(EXMEM_latch *exmem_input, MEMWB_latch *memwb_output, MEMWB_latch *memwb_input, int *MEM, REG *R, COUNT *counter, cacheline *cache)
{
	int v2;

	v2 = 0;
	if (exmem_input->control_signal.MemWrite || exmem_input->control_signal.MemRead)
	{
		counter->num_MEM_OP++;
		if (exmem_input->control_signal.MemRead) // lw
		{
			memwb_output->MEM_value = MemRead(exmem_input->ALU_rst, MEM, cache, counter);
			printf("[Memory]\tLW: R[%d] = MEM[0x%x] (%d)\n", exmem_input->debug.rt, exmem_input->ALU_rst, memwb_output->MEM_value);
		}
		else // sw dependency considering
		{
			if (exmem_input->debug.rt == memwb_input->Rdst && memwb_input->control_signal.RegWrite)
				v2 = memwb_input->ALU_rst;
			else
				v2 = R[exmem_input->debug.rt].value;
			MemWrite(exmem_input->ALU_rst, MEM, v2, cache, counter);
			printf("[Memory]\tSW: MEM[0x%x] = %d\n", exmem_input->ALU_rst, v2);
		}
	}
	else
		printf("[Memory]\t--\n");
	memwb_output->Rdst = exmem_input->Rdst;
	memwb_output->ALU_rst = exmem_input->ALU_rst;
	memwb_output->jump_target = exmem_input->jump_target;
	memwb_output->debug = exmem_input->debug;
	memwb_output->control_signal = exmem_input->control_signal;
}

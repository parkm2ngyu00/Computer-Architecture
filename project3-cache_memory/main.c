#include "cache.h"

int *INIT_MEM(void)
{
	int *MEM;
	int MEM_SIZE = INIT_SP / 4;

	if (!(MEM = (int *)malloc(sizeof(int) * MEM_SIZE)))
		return (0);
	return (MEM);
}

void update_latch(IFID_latch *ifid_latch, IDEX_latch *idex_latch, EXMEM_latch *exmem_latch, MEMWB_latch *memwb_latch)
{
	memcpy(&ifid_latch[1], &ifid_latch[0], sizeof(IFID_latch));
	memcpy(&idex_latch[1], &idex_latch[0], sizeof(IDEX_latch));
	memcpy(&exmem_latch[1], &exmem_latch[0], sizeof(EXMEM_latch));
	memcpy(&memwb_latch[1], &memwb_latch[0], sizeof(MEMWB_latch));

	memset(&ifid_latch[0], 0, sizeof(struct IFID_latch));
	memset(&idex_latch[0], 0, sizeof(struct IDEX_latch));
	memset(&exmem_latch[0], 0, sizeof(struct EXMEM_latch));
	memset(&memwb_latch[0], 0, sizeof(struct MEMWB_latch));
}

void print_result(int i, COUNT counter, REG *R)
{
	printf("\n\n============ Result ============\n");
	printf("Final return value:\t\t%d\n", R[2].value);
	printf("# of Total Cycle:\t\t%d\n", i);
	printf("# of Total Instruction:\t\t%d\n", counter.num_executed_inst);
	printf("# of R-type inst:\t\t%d\n", counter.num_R_type);
	printf("# of I-type inst:\t\t%d\n", counter.num_I_type);
	printf("# of J-type inst:\t\t%d\n", counter.num_J_type);
	printf("# of Memory access:\t\t%d\n", counter.num_MEM_OP);
	printf("# of jump:\t\t\t%d\n", counter.num_jump);
	printf("# of taken branch:\t\t%d\n", counter.num_taken_branch);
	printf("# of not taken branch:\t\t%d\n", counter.num_not_taken_branch);
	printf("# of cache access:\t\t%d\n", counter.num_cache_access);
	printf("# of cache hit:\t\t\t%d\n", counter.num_hit);
	printf("# of cache miss:\t\t%d\n", counter.num_miss);
	double hit_ratio = (double)counter.num_hit / (double)counter.num_cache_access;
	double miss_ratio = (double)counter.num_miss / (double)counter.num_cache_access;
	printf("cache hit ratio:\t\t%f\n", hit_ratio);
	printf("cache miss ratio:\t\t%f\n", miss_ratio);
}
cacheline cache[cache_size];

int main(int argc, char *argv[])
{
	int i;
	int *MEM;
	REG R[32];
	IFID_latch ifid_latch[2];
	IDEX_latch idex_latch[2];
	EXMEM_latch exmem_latch[2];
	MEMWB_latch memwb_latch[2];
	COUNT counter;
	memset(&counter, 0, sizeof(COUNT));
	BTB btb[10];
	for (int i = 0; i < 10; i++)
		memset(&btb[i], 0, sizeof(struct BTB));
	MEM = INIT_MEM();
	for (int i = 0; i < cache_size; i++)
		memset(&cache[i], 0, sizeof(struct cacheline));
	for (int i = 0; i < 32; i++)
	{
		if (i == 29)
		{
			R[i].value = INIT_SP;
			R[i].invalid = 0;
		}
		else if (i == 31)
		{
			R[i].value = INIT_LR;
			R[i].invalid = 0;
		}
		else
			memset(&R[i], 0, sizeof(struct REG));
	}
	memset(&ifid_latch[0], 0, sizeof(struct IFID_latch));
	memset(&ifid_latch[1], 0, sizeof(struct IFID_latch));
	memset(&idex_latch[0], 0, sizeof(struct IDEX_latch));
	memset(&idex_latch[1], 0, sizeof(struct IDEX_latch));
	memset(&exmem_latch[0], 0, sizeof(struct EXMEM_latch));
	memset(&exmem_latch[1], 0, sizeof(struct EXMEM_latch));
	memset(&memwb_latch[0], 0, sizeof(struct MEMWB_latch));
	memset(&memwb_latch[1], 0, sizeof(struct MEMWB_latch));
	g_branch_cnt = 0;
	i = 0;
	if (!load_file(argc, argv, MEM))
		return (0);
	while (1)
	{
		i++;
		if (i == 1)
			printf("\n===============%dst cycle===============\n\n", i);
		else if (i == 2)
			printf("\n===============%dnd cycle===============\n\n", i);
		else if (i == 3)
			printf("\n===============%drd cycle===============\n\n", i);
		else
			printf("\n===============%dth cycle===============\n\n", i);
		fetch(&ifid_latch[1], &ifid_latch[0], &idex_latch[1], &exmem_latch[1], btb, MEM, cache, &counter);
		decode(&ifid_latch[1], &ifid_latch[0], &idex_latch[0], R);
		execute(&ifid_latch[0], &idex_latch[1], &idex_latch[0], &exmem_latch[1], &exmem_latch[0], &memwb_latch[1], btb, &counter);
		memory_access(&exmem_latch[1], &memwb_latch[0], &memwb_latch[1], MEM, R, &counter, cache);
		write_back(&idex_latch[0], &exmem_latch[0], &memwb_latch[1], &memwb_latch[0], R);
		if (memwb_latch[1].jump_target == -1)
			break;
		update_latch(ifid_latch, idex_latch, exmem_latch, memwb_latch);
	}
	print_result(i, counter, R);
	return (0);
}

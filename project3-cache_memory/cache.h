#ifndef CACHE_H
#define CACHE_H
// header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// val
#define INIT_SP 0x1000000
#define INIT_LR 0xffffffff
#define cache_size 256
int g_branch_cnt;
// struct
typedef struct BTB
{
	int PC;
	int target_addr;
	int state_machine;
} BTB;
typedef struct REG
{
	int value;
	int invalid;
} REG;
typedef struct cacheline
{
	int tag;
	int sca;
	int valid;
	int dirty;
	int data[16];
} cacheline;
typedef struct IFID_latch
{
	int PC;
	int PC4;
	int instruction;
	int next_PC;
} IFID_latch;

typedef struct Debug
{
	int PC;
	int instruction;
	int rs;
	int rt;
	int rd;
	int opcode;
	int shamt;
	int funct;
	int imm;
	int addr;
} Debug;

typedef struct Control_Signal
{
	int PCSrc1;
	int PCSrc2;
	int RegDst;
	int Branch;
	int Jump;
	int MemRead;
	int MemtoReg;
	int MemWrite;
	int ALUSrc;
	int RegWrite;
	int V1Src;
	int V2Src;
} Control_Signal;

typedef struct IDEX_latch
{
	int next_PC;
	int PC4;
	int jump_target;
	int V1;
	int V2;
	int sign_imm;
	int Rdst;
	struct Debug debug;
	struct Control_Signal control_signal;
} IDEX_latch;

typedef struct EXMEM_latch
{
	int jump_target;
	int next_PC;
	int bcond;
	int ALU_rst;
	int V2;
	int Rdst;
	int prediction_fail;
	struct Debug debug;
	struct Control_Signal control_signal;
} EXMEM_latch;

typedef struct MEMWB_latch
{
	int ALU_rst;
	int Rdst;
	int MEM_value;
	int jump_target;
	int sw_src_reg;
	struct Debug debug;
	struct Control_Signal control_signal;
} MEMWB_latch;

typedef struct COUNT
{
	int num_executed_inst;
	int num_R_type;
	int num_I_type;
	int num_J_type;
	int num_REG_OP;
	int num_MEM_OP;
	int num_taken_branch;
	int num_not_taken_branch;
	int num_jump;
	int num_predict_O;
	int num_predict_X;
	int num_cache_access;
	int num_hit;
	int num_miss;
} COUNT;

// functions in MUX.c
int MUX_REGDst(int RegDst, int rd, int rt);
int MUX_ALUSrc(int ALUSrc, int imm, int rt_R_value);
int MUX_MemtoReg(int MemtoReg, int MEM_value, int ALU_rst);
int MUX_PCSrc1(int PCSrc1, int JumpAddr, int BR);
int MUX_PCSrc2(int PCSrc2, int PC, int BranchAddr);
int MUX_V1(int V1Src, int ALU_rst, int V1);
int MUX_V2(int V2Src, int ALU_rst, int V2);

// function in load.c
int load_file(int argc, char *argv[], int *MEM);
// function in fetch.c
void fetch(IFID_latch *ifid_input, IFID_latch *ifid_output, IDEX_latch *idex_input, EXMEM_latch *exmem_input, BTB *btb, int *MEM, cacheline *cache, COUNT *counter);
// function in decode.c
void decode(IFID_latch *ifid_input, IFID_latch *ifid_output, IDEX_latch *idex_output, REG *R);
char format(int OPCODE);
// function in execute.c
void execute(IFID_latch *ifid_output, IDEX_latch *idex_input, IDEX_latch *idex_output, EXMEM_latch *exmem_input, EXMEM_latch *exmem_output, MEMWB_latch *memwb_input, BTB *btb, COUNT *counter);
// function in memory_access.c
void memory_access(EXMEM_latch *exmem_input, MEMWB_latch *memwb_output, MEMWB_latch *memwb_input, int *MEM, REG *R, COUNT *counter, cacheline *cache);
// function in write_back.c
void write_back(IDEX_latch *idex_output, EXMEM_latch *exmem_output, MEMWB_latch *memwb_input, MEMWB_latch *memwb_output, REG *R);
// functions in direct_mapped_cache.c
int MemRead(int addr, int *MEM, cacheline *cache, COUNT *counter);
void MemWrite(int addr, int *MEM, int value, cacheline *cache, COUNT *counter);
#endif

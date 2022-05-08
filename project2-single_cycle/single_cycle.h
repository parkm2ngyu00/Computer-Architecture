#ifndef SINGLE_CYCLE_H
#define SINGLE_CYCLE_H

// header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// val
#define INIT_SP 0x1000000
#define INIT_LR 0xffffffff

// struct
typedef struct BASIC_INST_FORMAT
{
    unsigned int RS;
    unsigned int RT;
    unsigned int RD;
    unsigned int SHAMT;
    unsigned int FUNCT;
    unsigned int IMM;
    unsigned int ADDR;
} inst_fmt;

typedef struct CONTROL_SIGNAL
{
    int PCSrc1;
    int PCSrc2;
    int bcond;
    int RegDst;
    int Branch;
    int MemRead;
    int MemtoReg;
    int MemWrite;
    int ALUSrc;
    int RegWrite;
} signal;

typedef struct COUNT
{
    int num_executed_inst;
    int num_R_type;
    int num_I_type;
    int num_J_type;
    int num_MEM_access;
    int num_taken_branch;
} CNT;

// functions in fetch.c
int SET_INST_MEM(int argc, char *argv[], int *MEM);
unsigned int FETCH(unsigned int PC, int *MEM);
// functions in decode.c
char format(unsigned OPCODE);
inst_fmt DECODE(unsigned int INSTRUCTION, unsigned int OPCODE, signal *SIG);
// functions in execute.c
int calc_SignExtImm(unsigned int imm);
int calc_BranchAddr(unsigned int imm);
int calc_JumpAddr(unsigned int addr, unsigned int PC);
int EXECUTE(inst_fmt FMT, int *R, unsigned int OPCODE, unsigned int PC, CNT *cnt, signal SIG);
// functions in memory_access.c
int MEMORY_ACCESS(int *MEM, inst_fmt FMT, int *R, CNT *cnt, signal SIG, int ALU_rst);
// functions in write_back.c
void WB(int *R, int ALU_rst, signal SIG, int MEM_value, inst_fmt FMT);
// functions in MUX.c
int MUX_REGDst(signal SIG, inst_fmt FMT);
int MUX_ALUSrc(signal SIG, inst_fmt FMT, int *R);
int MUX_MemtoReg(signal SIG, int MEM_value, int ALU_rst);
int MUX_PCSrc1(signal SIG, int JumpAddr, int BR);
int MUX_PCSrc2(signal SIG, unsigned int PC, int BranchAddr);

#endif

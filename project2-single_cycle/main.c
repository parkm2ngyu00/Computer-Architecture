#include "single_cycle.h"

int *INIT_MEM(void)
{
    int *MEM;

    int MEM_SIZE = INIT_SP / 4;
    if (!(MEM = (int *)malloc(sizeof(int) * MEM_SIZE)))
        return (0);
    return (MEM);
}

int *INIT_REG(void)
{
    int *R;

    if (!(R = (int *)malloc(sizeof(int) * 32)))
        return (0);
    R[29] = INIT_SP;
    R[31] = INIT_LR;
    return (R);
}

void INIT_CNT(CNT *cnt)
{
    cnt->num_executed_inst = 0;
    cnt->num_R_type = 0;
    cnt->num_I_type = 0;
    cnt->num_J_type = 0;
    cnt->num_MEM_access = 0;
    cnt->num_taken_branch = 0;
}

void INIT_control_signal(signal *SIG)
{
    SIG->PCSrc1 = 0;
    SIG->PCSrc2 = 0;
    SIG->bcond = 0;
    SIG->RegDst = 0;
    SIG->Branch = 0;
    SIG->MemRead = 0;
    SIG->MemtoReg = 0;
    SIG->MemWrite = 0;
    SIG->ALUSrc = 0;
    SIG->RegWrite = 0;
}

int PC_Change(inst_fmt FMT, signal SIG, CNT *cnt, unsigned int OPCODE, unsigned int PC, int ALU_rst, int *R)
{
    int JumpAddr;
    int BranchAddr;
    int BR;

    if (SIG.Branch && ALU_rst) // ALU_rst is evaluated in ALU, 1 or 0 (in case of SIG.Branch == 1)
    {
        cnt->num_taken_branch++;
        SIG.PCSrc2 = 1;
    }
    JumpAddr = calc_JumpAddr(FMT.ADDR, PC);
    BranchAddr = calc_BranchAddr(FMT.IMM);
    BR = MUX_PCSrc2(SIG, PC, BranchAddr);
    PC = MUX_PCSrc1(SIG, JumpAddr, BR);
    if ((OPCODE == 0) && (FMT.FUNCT == 0x8))
        PC = ALU_rst;
    if ((OPCODE == 0) && (FMT.FUNCT == 0x9))
        PC = R[FMT.RS];
    return (PC);
}

void print_final(int *R, CNT cnt)
{
    printf("Final return value:\t\t\t%d\n", R[2]);
    printf("Number of executed instructions:\t%d\n", cnt.num_executed_inst);
    printf("Number of R-type instructions:\t\t%d\n", cnt.num_R_type);
    printf("Number of I-type instructions:\t\t%d\n", cnt.num_I_type);
    printf("Number of J-type instructions:\t\t%d\n", cnt.num_J_type);
    printf("Number of memory access instructions: \t%d\n", cnt.num_MEM_access);
    printf("Number of taken branches:\t\t%d\n", cnt.num_taken_branch);
}

int main(int argc, char *argv[])
{
    int *MEM;
    int *R;
    int ALU_rst;
    int MEM_value;
    unsigned int OPCODE;
    unsigned int PC;
    unsigned int INSTRUCTION;
    inst_fmt FMT;
    signal SIG;
    CNT cnt;

    MEM = INIT_MEM();
    R = INIT_REG();
    INIT_CNT(&cnt);
    if (!SET_INST_MEM(argc, argv, MEM))
        return (0);
    PC = 0;
    ALU_rst = 0;
    MEM_value = 0;
    while (PC != 0xffffffff)
    {
        INIT_control_signal(&SIG);
        INSTRUCTION = FETCH(PC, MEM);
        OPCODE = (INSTRUCTION & 0xfc000000) >> 26;
        FMT = DECODE(INSTRUCTION, OPCODE, &SIG);
        ALU_rst = EXECUTE(FMT, R, OPCODE, PC, &cnt, SIG);
        MEM_value = MEMORY_ACCESS(MEM, FMT, R, &cnt, SIG, ALU_rst);
        WB(R, ALU_rst, SIG, MEM_value, FMT);
        PC = PC_Change(FMT, SIG, &cnt, OPCODE, PC, ALU_rst, R);
        printf("updated PC: 0x%x\n\n", PC);
    }
    printf("PC: 0xffffffff, HALT!\n==========================================\n");
    print_final(R, cnt);
    // free
    free(MEM);
    free(R);
    return (0);
}

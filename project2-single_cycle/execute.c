#include "single_cycle.h"

int calc_SignExtImm(unsigned int imm)
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

int calc_BranchAddr(unsigned int imm)
{
    imm = (imm * 4);
    if (imm >= 0x8000)
        imm = imm | 0xfffc0000;
    return (imm);
}

int calc_JumpAddr(unsigned int addr, unsigned int PC)
{
    PC = (PC + 4) & 0xf0000000;
    addr = (addr * 4) & 0x0ffffffc;
    addr = addr + PC;
    return (addr);
}

int ALUOP(inst_fmt FMT, signal SIG, unsigned int OPCODE, unsigned int PC, int *R)
{
    int rst;
    int SignExtImm;
    int ZeroExtImm;
    int Read_data1;
    int Read_data2;

    rst = 0;
    if (OPCODE == 0x3)
    {
        rst = PC + 8;
        return (rst);
    }
    Read_data1 = R[FMT.RS];
    Read_data2 = MUX_ALUSrc(SIG, FMT, R);
    SignExtImm = calc_SignExtImm(FMT.IMM);
    ZeroExtImm = FMT.IMM;
    if (FMT.FUNCT == 0x8)
        rst = Read_data1;
    else if (FMT.FUNCT == 0x20)
        rst = Read_data1 + Read_data2;
    else if (FMT.FUNCT == 0x21)
        rst = Read_data1 + Read_data2;
    else if (FMT.FUNCT == 0x24)
        rst = Read_data1 & Read_data2;
    else if (FMT.FUNCT == 0x27)
        rst = ~(Read_data1 | Read_data2);
    else if (FMT.FUNCT == 0x25)
        rst = Read_data1 | Read_data2;
    else if (FMT.FUNCT == 0x2a)
        rst = (Read_data1 < Read_data2) ? 1 : 0;
    else if (FMT.FUNCT == 0x2b)
        rst = (Read_data1 < Read_data2) ? 1 : 0;
    else if (FMT.FUNCT == 0x00)
        rst = Read_data2 << FMT.SHAMT;
    else if (FMT.FUNCT == 0x02)
        rst = Read_data2 >> FMT.SHAMT;
    else if (FMT.FUNCT == 0x22)
        rst = Read_data1 - Read_data2;
    else if (FMT.FUNCT == 0x23)
        rst = Read_data1 - Read_data2;
    else if (FMT.FUNCT == 0x9)
        rst = PC + 4;
    else if (OPCODE == 0x8)
        rst = Read_data1 + SignExtImm;
    else if (OPCODE == 0x9)
        rst = Read_data1 + SignExtImm;
    else if (OPCODE == 0xc)
        rst = Read_data1 & ZeroExtImm;
    else if (OPCODE == 0xf)
        rst = Read_data2 << 16;
    else if (OPCODE == 0xd)
        rst = Read_data1 | ZeroExtImm;
    else if (OPCODE == 0xa)
        rst = (Read_data1 < SignExtImm) ? 1 : 0;
    else if (OPCODE == 0xb)
        rst = (Read_data1 < SignExtImm) ? 1 : 0;
    else if (OPCODE == 0x23)
        rst = (Read_data1 + SignExtImm);
    else if (OPCODE == 0x2b)
        rst = (Read_data1 + SignExtImm);
    else if (OPCODE == 0x4)
        rst = (Read_data1 == Read_data2);
    else if (OPCODE == 0x5)
        rst = (Read_data1 != Read_data2);
    else if (OPCODE == 0x3)
        rst = PC + 8;
    return (rst);
}

int EXECUTE(inst_fmt FMT, int *R, unsigned int OPCODE, unsigned int PC, CNT *cnt, signal SIG)
{
    int rst;

    rst = 0;
    cnt->num_executed_inst++;
    if (format(OPCODE) == 'R')
    {
        rst = ALUOP(FMT, SIG, OPCODE, PC, R);
        printf("[EXE]\tALU RESULT: 0x%x (= %d)\n", rst, rst);
        cnt->num_R_type++;
        return (rst);
    }
    else if (format(OPCODE) == 'I')
    {
        rst = ALUOP(FMT, SIG, OPCODE, PC, R);
        printf("[EXE]\tALU RESULT: 0x%x (= %d)\n", rst, rst);
        cnt->num_I_type++;
        return (rst);
    }
    else if (SIG.PCSrc1)
    {
        cnt->num_J_type++;
        if (OPCODE == 0x3)
        {
            rst = ALUOP(FMT, SIG, OPCODE, PC, R);
            printf("[EXE]\tALU RESULT: 0x%x (= %d)\n", rst, rst);
            return (rst);
        }
    }
    return (0);
}
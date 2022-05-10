#include "single_cycle.h"

char format(unsigned OPCODE)
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

void set_inst_fmt(inst_fmt *FMT, unsigned int inst, unsigned int OPCODE, char format)
{
    if (format == 'R')
    {
        FMT->RS = (inst & 0x03e00000) >> 21;
        FMT->RT = (inst & 0x001f0000) >> 16;
        FMT->RD = (inst & 0x0000f800) >> 11;
        FMT->SHAMT = (inst & 0x000007c0) >> 6;
        FMT->FUNCT = (inst & 0x0000003f);
        printf("[ID]\tR-type\n\tOPCODE: 0x%x\tRS: %d\tRT: %d\tRD: %d\tSHAMT: 0x%x\tFUNCT: 0x%02x\n", OPCODE, (*FMT).RS, (*FMT).RT, (*FMT).RD, (*FMT).SHAMT, (*FMT).FUNCT);
    }
    else if (format == 'I')
    {
        FMT->RS = (inst & 0x03e00000) >> 21;
        FMT->RT = (inst & 0x001f0000) >> 16;
        FMT->IMM = (inst & 0x0000ffff);
        printf("[ID]\tI-type\n\tOPCODE: 0x%x\tRS: %d\tRT: %d\tIMM: 0x%x\n", OPCODE, (*FMT).RS, (*FMT).RT, (*FMT).IMM);
    }
    else if (format == 'J')
    {
        FMT->ADDR = (inst & 0x03ffffff);
        printf("[ID]\tJ-type\n\tOPCODE: 0x%x\tADDR: 0x%08x\n", OPCODE, (*FMT).ADDR);
    }
}

void set_control_signal(unsigned int OPCODE, inst_fmt FMT, signal *SIG)
{
    if (OPCODE == 0)
        SIG->RegDst = 1;
    if ((OPCODE != 0) && (OPCODE != 0x4) && (OPCODE != 0x5))
        SIG->ALUSrc = 1;
    if (OPCODE == 0x23)
    {
        SIG->MemtoReg = 1;
        SIG->MemRead = 1;
    }
    if ((OPCODE != 0x2b) && (OPCODE != 0x4) && (OPCODE != 0x5) && (OPCODE != 0x2) && !((OPCODE == 0) && (FMT.FUNCT == 0x8)))
        SIG->RegWrite = 1;
    if (OPCODE == 0x2b)
        SIG->MemWrite = 1;
    if ((OPCODE == 0x2) || (OPCODE == 0x3))
        SIG->PCSrc1 = 1;
    if ((OPCODE == 0x4) || (OPCODE == 0x5))
        SIG->Branch = 1;
}

inst_fmt DECODE(unsigned int INSTRUCTION, unsigned int OPCODE, signal *SIG)
{
    inst_fmt fmt;

    if (!format(OPCODE))
    {
        printf("ERROR: Invalid OPCODE.\n");
        return (fmt);
    }
    set_inst_fmt(&fmt, INSTRUCTION, OPCODE, format(OPCODE));
    set_control_signal(OPCODE, fmt, SIG);
    return (fmt);
}
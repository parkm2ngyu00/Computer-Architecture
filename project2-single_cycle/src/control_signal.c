#include "single_cycle.h"

int MUX_REGDst(sig SIG, inst_fmt FMT)
{
    if (SIG.RegDst)
        return (FMT.RD);
    else
        return (FMT.RT);
}

int MUX_ALUSrc(sig SIG, inst_fmt FMT, int *R)
{
    if (SIG.ALUSrc)
        return (FMT.IMM);
    else
        return (R[FMT.RT]);
}

int MUX_MemtoReg(sig SIG, int MEM_value, int ALU_rst)
{
    if (SIG.MemtoReg)
        return (MEM_value);
    else
        return (ALU_rst);
}

int MUX_PCSrc1(sig SIG, int JumpAddr, int BR)
{
    if (SIG.PCSrc1)
        return (JumpAddr);
    else
        return (BR);
}

int MUX_PCSrc2(sig SIG, unsigned int PC, int BranchAddr)
{
    if (SIG.PCSrc2)
        return (PC + 4 + BranchAddr);
    else
        return (PC + 4);
}
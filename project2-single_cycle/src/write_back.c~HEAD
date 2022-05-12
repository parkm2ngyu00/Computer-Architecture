#include "single_cycle.h"

void WB(int *R, int ALU_rst, signal SIG, int MEM_value, inst_fmt FMT)
{
    int rst;
    int Write_Reg;

    rst = MUX_MemtoReg(SIG, MEM_value, ALU_rst);
    Write_Reg = MUX_REGDst(SIG, FMT);
    if (SIG.RegWrite)
    {
        if (SIG.PCSrc1)
        {
            R[31] = rst;
            printf("[WB]\tR[31] = 0x%x\n", rst);
        }
        else
        {
            R[Write_Reg] = rst;
            printf("[WB]\tR[%d] = 0x%x\n", Write_Reg, rst);
        }
    }
}

#include "single_cycle.h"

int MEMORY_ACCESS(int *MEM, inst_fmt FMT, int *R, CNT *cnt, signal SIG, int ALU_rst)
{
    int MEM_value;

    MEM_value = 0;
    if (SIG.MemWrite || SIG.MemRead)
    {
        cnt->num_MEM_access++;
        if (SIG.MemRead) //  lw
        {
            MEM_value = MEM[ALU_rst / 4];
            printf("[MEM]\tLW: R[%d] = MEM[0x%x] (%d)\n", FMT.RT, ALU_rst, MEM_value);
        }
        else // sw
        {
            printf("[MEM]\tSW: MEM[0x%x] = R[%d] (%d)\n", ALU_rst, FMT.RT, R[FMT.RT]);
            MEM[ALU_rst / 4] = R[FMT.RT];
        }
    }
    return (MEM_value);
}
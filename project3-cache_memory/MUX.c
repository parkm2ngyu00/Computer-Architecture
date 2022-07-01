#include "cache.h"

int MUX_REGDst(int RegDst, int rd, int rt)
{
	if (RegDst)
		return (rd);
	else
		return (rt);
}

int MUX_ALUSrc(int ALUSrc, int imm, int rt_R_value)
{
	if (ALUSrc)
		return (imm);
	else
		return (rt_R_value);
}

int MUX_MemtoReg(int MemtoReg, int MEM_value, int ALU_rst)
{
	if (MemtoReg)
		return (MEM_value);
	else
		return (ALU_rst);
}

int MUX_PCSrc1(int PCSrc1, int JumpAddr, int BR)
{
	if (PCSrc1)
		return (JumpAddr);
	else
		return (BR);
}

int MUX_PCSrc2(int PCSrc2, int PC, int BranchAddr)
{
	if (PCSrc2)
		return (PC + 4 + BranchAddr);
	else
		return (PC + 4);
}

int MUX_V1(int V1Src, int ALU_rst, int V1)
{
	if (V1Src)
		return (ALU_rst);
	else
		return (V1);
}

int MUX_V2(int V2Src, int ALU_rst, int V2)
{
	if (V2Src)
		return (ALU_rst);
	else
		return (V2);
}

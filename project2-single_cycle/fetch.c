#include "single_cycle.h"

int input_is_error(int argc, char *argv[], FILE *fp)
{
    int arr_size;

    if (argc < 2)
    {
        printf("ERROR: There is no input file.\n");
        return (1);
    }
    else if (argc > 2)
    {
        printf("ERROR: There are too many input files.\n");
        return (1);
    }
    arr_size = strlen(argv[1]) - 1;
    if (!((argv[1][arr_size - 3] == '.') &&
          (argv[1][arr_size - 2] == 'b') &&
          (argv[1][arr_size - 1] == 'i') &&
          (argv[1][arr_size] == 'n')))
    {
        printf("ERROR: This is not a .bin file.\n");
        return (1);
    }
    else if (fp == NULL)
    {
        printf("ERROR: This is an empty file.\n");
        return (1);
    }
    else
        return (0);
}

void swap(unsigned char *data)
{
    unsigned char tmp;
    // data[0] <-> data[3]
    tmp = data[3];
    data[3] = data[0];
    data[0] = tmp;
    // data[1] <-> data[2]
    tmp = data[2];
    data[2] = data[1];
    data[1] = tmp;
}

int *read_origin_inst(FILE *fp)
{
    unsigned char c_data[4];

    for (int i = 0; i < 4; i++)
        fread(&c_data[i], sizeof(unsigned char), 1, fp);
    swap(c_data);
    int *p_rtn = (int *)&c_data;
    return (p_rtn);
}

int SET_INST_MEM(int argc, char *argv[], int *MEM)
{
    FILE *fp;
    int *p_int;
    int line;

    fp = fopen(argv[1], "rb");
    if (input_is_error(argc, argv, fp))
        return (0);
    line = 0;
    while (!feof(fp))
    {
        p_int = read_origin_inst(fp);
        MEM[line] = *p_int;
        printf("0x%x:\t0x%08x\n", line * 4, MEM[line]);
        line++;
    }
    printf("=== Finish Set Instruction Memory from %s ===\n\n", argv[1]);
    fclose(fp);
    return (1);
}

unsigned int FETCH(unsigned int PC, int *MEM)
{
    unsigned int instruction;

    instruction = MEM[PC / 4];
    printf("[IF]\tPC: 0x%x -> INSTRUCTION: 0x%08x\n", PC, instruction);
    return (instruction);
}

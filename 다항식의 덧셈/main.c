#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "instructions.h"
#define MAX_LEN 100

long R0, R1, R2, R3, R4, R5, R6, R7, R8, R9;

int main(void)
{
    char operation[3][MAX_LEN];
    FILE *fs;
    fs = fopen("input.txt", "r");
    while (feof(fs) == 0)
    {
        char str[MAX_LEN];
        fgets(str, MAX_LEN, fs);
        char *ptr = strtok(str, " ");
        int i = 0;
        if (ptr[0] == 'H')
            break;
        while (ptr != NULL)
        {
            strcpy(operation[i], ptr);
            ptr = strtok(NULL, " ");
            i++;
        }
        if (operation[0][0] == '+' || operation[0][0] == '-' || operation[0][0] == '*' || operation[0][0] == '/')
        {
            long R1 = strtol(operation[1], NULL, 16);
            long R2 = strtol(operation[2], NULL, 16);
            R0 = calculater(operation[0][0], R1, R2);
        }
        else if (operation[0][0] == 'M')
        {
            long R0 = strtol(operation[2], NULL, 16);
            move(operation[1][1], R0);
        }
    }
    fclose(fs);
    return 0;
}

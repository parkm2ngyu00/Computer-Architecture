#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "instructions.h"
#define MAX_LEN 100

int main(void)
{
    char operation[3][MAX_LEN];
    long R0, R1, R2;
    FILE *fs;
    fs = fopen("input.txt", "r");
    while (feof(fs) == 0)
    {
        char str[MAX_LEN];
        fgets(str, MAX_LEN, fs);
        char *ptr = strtok(str, " ");
        int i = 0;
        while (ptr != NULL)
        {
            strcpy(operation[i], ptr);
            ptr = strtok(NULL, " ");
            i++;
        }
        if (operation[0][0] == 'M')
        {

            long R2 = strtol(operation[2], NULL, 16);
        }
        long R1 = strtol(operation[1], NULL, 16);
        long R2 = strtol(operation[2], NULL, 16);
        switch (operation[0][0])
        {
        case '+':
            R0 = ft_add(R1, R2);
            printf("R0 : %d = %d %c %d\n", R0, R1, operation[0][0], R2);
            break;
        case '-':
            R0 = ft_sub(R1, R2);
            printf("R0 : %d = %d %c %d\n", R0, R1, operation[0][0], R2);
            break;
        case '*':
            R0 = ft_mul(R1, R2);
            printf("R0 : %d = %d %c %d\n", R0, R1, operation[0][0], R2);
            break;
        case '/':
            R0 = ft_div(R1, R2);
            printf("R0 : %d = %d %c %d\n", R0, R1, operation[0][0], R2);
            break;
        default:
            break;
        }
    }
    fclose(fs);

    return 0;
}

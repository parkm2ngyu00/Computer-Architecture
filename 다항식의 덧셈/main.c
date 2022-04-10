#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "instructions.h"
#define MAX_LEN 100

long R0, R1, R2, R3, R4, R5, R6, R7, R8, R9;

char buffer[MAX_LEN];

int getTotalLine(char *name)
{
    FILE *fp;
    int line = 0;
    char c;
    fp = fopen(name, "r");
    while ((c = fgetc(fp)) != EOF)
        if (c == '\n')
            line++;
    fclose(fp);
    return (line);
}

void move(char index, long result)
{
    switch (index)
    {
    case '1':
        R1 = result;
        break;
    case '2':
        R2 = result;
        break;
    case '3':
        R3 = result;
        break;
    case '4':
        R4 = result;
        break;
    case '5':
        R5 = result;
        break;
    case '6':
        R6 = result;
        break;
    case '7':
        R7 = result;
        break;
    case '8':
        R8 = result;
        break;
    case '9':
        R9 = result;
        break;
    default:
        break;
    }
    printf("R%c : %d\n", index, result);
}

void openAndReadFile(int line)
{
    int totalLine = getTotalLine("input.txt") + 1;
    char operation[3][MAX_LEN];
    int currentLine = 1;
    FILE *fs;
    fs = fopen("input.txt", "r");
    while (fgets(buffer, MAX_LEN, fs) != NULL)
    {
        if (currentLine < line)
        {
            currentLine += 1;
            continue;
        }
        char *ptr = strtok(buffer, " ");
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
            R1 = strtol(operation[1], NULL, 16);
            R2 = strtol(operation[2], NULL, 16);
            R0 = calculater(operation[0][0], R1, R2);
        }
        else if (operation[0][0] == 'M')
        {
            R9 = strtol(operation[2], NULL, 16);
            move(operation[1][1], R9);
        }
        else if (operation[0][0] == 'C')
        {
            R1 = strtol(operation[1], NULL, 16);
            R2 = strtol(operation[2], NULL, 16);
            R0 = compare(R1, R2);
            printf("R0 : %d\n", R0);
        }
        else if (operation[0][0] == 'J')
        {
            R9 = strtol(operation[1], NULL, 16);
            fclose(fs);
            openAndReadFile(R9);
        }
        else if (operation[0][0] == 'B')
        {
        }
        currentLine += 1;
    }
    fclose(fs);
};

int main(void)
{
    openAndReadFile(1);
    return 0;
}

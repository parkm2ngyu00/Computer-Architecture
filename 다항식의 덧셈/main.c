#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "instructions.h"
#define MAX_LEN 100

long R0, R1, R2, R3, R4, R5, R6, R7, R8, R9;

int getTotalLine(char *name) {
  FILE *fp;
  int line=0;
  char c;
  fp=fopen(name,"r");
  while((c=fgetc(fp)) != EOF)
    if(c=='\n')
        line++;
  fclose(fp);
  return(line);
}

void move(char index, long result)
{
    switch (index)
    {
    case '1':
        R1 = result;
        printf("R1 : %d\n", R1);
        break;
    case '2':
        R2 = result;
        printf("R2 : %d\n", R2);
        break;
    case '3':
        R3 = result;
        printf("R3 : %d\n", R3);
        break;
    case '4':
        R4 = result;
        printf("R4 : %d\n", R4);
        break;
    case '5':
        R5 = result;
        printf("R5 : %d\n", R5);
        break;
    case '6':
        R6 = result;
        printf("R6 : %d\n", R6);
        break;
    case '7':
        R7 = result;
        printf("R7 : %d\n", R7);
        break;
    case '8':
        R8 = result;
        printf("R8 : %d\n", R8);
        break;
    case '9':
        R9 = result;
        printf("R9 : %d\n", R9);
        break;
    default:
        break;
    }
}

int main(void)
{
    int totalLine = getTotalLine("input.txt") + 1;
    char operation[2][MAX_LEN];
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
            R1 = strtol(operation[1], NULL, 16);
            R2 = strtol(operation[2], NULL, 16);
            R0 = calculater(operation[0][0], R1, R2);
        }
        else if (operation[0][0] == 'M')
        {
            R9 = strtol(operation[2], NULL, 16);
            move(operation[1][1], R9);
        }
        else if (operation[0][0] == 'C') {
            R1 = strtol(operation[1], NULL, 16);
            R2 = strtol(operation[2], NULL, 16);
            R0 = compare(R1, R2);
            printf("R0 : %d\n", R0);
        }
        else if (operation[0][0] == 'J') {

        }
        else if (operation[0][0] == 'B') {
            
        }
    }
    fclose(fs);
    return 0;
}

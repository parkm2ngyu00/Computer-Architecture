#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "instructions.h"
#define MAX_LEN 100

static long R0, R1, R2, R3, R4, R5, R6, R7, R8, R9;

char buffer[MAX_LEN];

// 읽어올 .txt파일 라인의 총 개수 반환
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

// 변수에 값은 대입하는 Move instruction
void move(char index, long result)
{
    switch (index)
    {
    case '0':
        R0 = result;
        break;
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

// main logic
// 매개변수로 받은 값의 줄부터 파일 read를 시작한다(나중에 Jump instruction을 위해 쓰인다.)
void openAndReadFile(int line)
{
    int totalLine = getTotalLine("gcd.txt") + 1;
    char operation[3][MAX_LEN];
    int currentLine = 1;
    FILE *fs;
    fs = fopen("gcd.txt", "r");
    while (fgets(buffer, MAX_LEN, fs) != NULL)
    {
        // 매개변수로 받은 line 값보다 현재 currentLine 이 작다면, 다음 줄로 넘어간다.
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
        // calculater
        // 계산을 할 때는 무조건 R1, R2레지스터를 이용해서 한다.
        if (operation[0][0] == '+' || operation[0][0] == '-' || operation[0][0] == '*' || operation[0][0] == '/')
        {
            if (operation[1][1] == 'x')
                R1 = strtol(operation[1], NULL, 16);
            if (operation[2][1] == 'x')
                R2 = strtol(operation[2], NULL, 16);
            R0 = calculater(operation[0][0], R1, R2);
        }
        // Move
        else if (operation[0][0] == 'M')
        {
            switch (operation[2][1])
            {
            case 'x':
                R0 = strtol(operation[2], NULL, 16);
                move(operation[1][1], R0);
                break;
            case '0':
                move(operation[1][1], R0);
                break;
            case '1':
                move(operation[1][1], R1);
                break;
            case '2':
                move(operation[1][1], R2);
                break;
            case '3':
                move(operation[1][1], R3);
                break;
            case '4':
                move(operation[1][1], R4);
                break;
            case '5':
                move(operation[1][1], R5);
                break;
            case '6':
                move(operation[1][1], R6);
                break;
            case '7':
                move(operation[1][1], R7);
                break;
            case '8':
                move(operation[1][1], R8);
                break;
            case '9':
                move(operation[1][1], R9);
                break;
            default:
                break;
            }
        }
        // Compare
        else if (operation[0][0] == 'C')
        {
            if (operation[1][1] == 'x')
                R1 = strtol(operation[1], NULL, 16);
            if (operation[2][1] == 'x')
                R2 = strtol(operation[2], NULL, 16);
            R0 = compare(R1, R2);
            printf("R0 : %d\n", R0);
        }
        // Jump
        else if (operation[0][0] == 'J')
        {
            R9 = strtol(operation[1], NULL, 16);
            if (R9 <= totalLine) {
                fclose(fs);
                openAndReadFile(R9);
            }
        }
        // Branch
        else if (operation[0][0] == 'B')
        {
            if (R0 == 1) {
                R9 = strtol(operation[1], NULL, 16);
                if (R9 <= totalLine) {
                    fclose(fs);
                    openAndReadFile(R9);
                }
            }
        }
        // 'Same' instruction for gcd
        else if (operation[0][0] == 'S') {
            R0 = same(R1, R2);
            printf("R0 : %d\n", R0);
        }
        // GCD start
        else if (strcmp(operation[0], "gcd") == 0) {
            R1 = strtol(operation[1], NULL, 16);
            R2 = strtol(operation[2], NULL, 16);
            printf("a : %d, b : %d\n", R1, R2);
        }
        currentLine += 1;
    }
    fclose(fs);
};

// 메인함수에서는 첫번째 줄부터 .txt파일을 읽으라고 명령한다.
int main(void)
{
    openAndReadFile(1);
    return 0;
}

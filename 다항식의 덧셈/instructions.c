#include <stdio.h>

long R0, R1, R2, R3, R4, R5, R6, R7, R8, R9;

long ft_add(long x, long y)
{
    return (x + y);
}

long ft_sub(long x, long y)
{
    return (x - y);
}

long ft_mul(long x, long y)
{
    return (x * y);
}

long ft_div(long x, long y)
{
    return (x / y);
}

long calculater(char operation, long x, long y)
{
    long result;
    switch (operation)
    {
    case '+':
        result = ft_add(x, y);
        printf("R0 : %d = %d %c %d\n", result, x, operation, y);
        break;
    case '-':
        result = ft_sub(x, y);
        printf("R0 : %d = %d %c %d\n", result, x, operation, y);
        break;
    case '*':
        result = ft_mul(x, y);
        printf("R0 : %d = %d %c %d\n", result, x, operation, y);
        break;
    case '/':
        result = ft_div(x, y);
        printf("R0 : %d = %d %c %d\n", result, x, operation, y);
        break;
    default:
        break;
    }
    return result;
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
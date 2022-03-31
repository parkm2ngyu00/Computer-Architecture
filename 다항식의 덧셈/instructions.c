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

long compare(long x, long y) {
    return (x >= y) ? 1 : 0;
}
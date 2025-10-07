#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int gNumStack[100];
int gNumTop = -1;

char gOpStack[100];
int gOpTop = -1;

void pushNum(int num)
{
    gNumStack[++gNumTop] = num;
}

void pushOp(char op)
{
    gOpStack[++gOpTop] = op;
}

int numPop(){

    return gNumStack[gNumTop--];
}

char opPop()
{
    return gOpStack[gOpTop--];
}

int precedance(char c)
{
    if(c == '+' || c == '-')
    {
        return 1;
    }
    else if(c == '*' || c == '/')
    {
        return 2;
    }
    return -1;
}

int calculate(int num1, int num2, char op, int *errorPtr)
{
    if((num1 == 0 || num2 == 0) && op == '/')
    {
        *errorPtr = 1;
        printf("Invalid Division By Zero");
        return -1;
    }
    if(op == '+') return num1 + num2;
    if(op == '-') return num1 - num2;
    if(op == '*') return num1 * num2;
    if(op == '/') return num1 / num2;
}

int evaluate(char expression[])
{
    int len = strlen(expression), errorPtr = 0;
    if(len == 0)
    {
        printf("Invalid Expression.\n");
        gOpTop++;
        return 0;
    }
    for(int i = 0; i < len; i++)
    {
        if(isspace(expression[i]))
        {
            continue;
        }

        if(isdigit(expression[i]))
        {
            int val = 0;
            while(isdigit(expression[i]))
            {
                val = val * 10 + (expression[i] - '0');
                i++;
            }
            pushNum(val);
            i--;
        }
        else if(expression[i] == '-' || expression[i] == '+' || expression[i] == '/' || expression[i] == '*')
        {
            while(gOpTop != -1 && precedance(expression[i]) <= precedance(gOpStack[gOpTop]))
            {
                int num1 = numPop();
                int num2 = numPop();
                char op = opPop();
                int result = calculate(num1, num2, op, &errorPtr);
                if(errorPtr == 0)
                {
                    pushNum(result);
                }
                else
                {
                    printf("Error: Division by zero.");
                    return 0;
                }
            }
            pushOp(expression[i]);
        }
        else
        {
            printf("'Error: Invalid expression.");
            return 0;
        }
    }
    while(gOpTop != -1)
    {
        int num1 = numPop();
        int num2 = numPop();
        char op = opPop();
        int result = calculate(num1, num2, op, &errorPtr);
        if(result != -1)
        {
            pushNum(result);
        }
    }
    return gNumStack[gNumTop];
}

int main()
{
    char expression[100];
    scanf("%[^\n]", expression);
    int result = evaluate(expression);
    if(gNumTop != 0 || gOpTop != -1)
    {
        return 1;
    }
    printf("%d", result);
    return 0;
}
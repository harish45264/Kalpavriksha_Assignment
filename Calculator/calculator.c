#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int gNumStack[100];
int gNumTop = -1;

char gOpStack[100];
int gOpTop = -1;

void pushNum(int number)
{
    gNumStack[++gNumTop] = number;
}

void pushOp(char operator)
{
    gOpStack[++gOpTop] = operator;
}

int numPop(){

    return gNumStack[gNumTop--];
}

char opPop()
{
    return gOpStack[gOpTop--];
}

int precedance(char operator)
{
    if(operator == '+' || operator == '-')
    {
        return 1;
    }
    else if(operator == '*' || operator == '/')
    {
        return 2;
    }
    return -1;
}

int calculate(int number1, int number2, char operator, int *errorPtr)
{
    if((number1 == 0 || number2 == 0) && operator == '/')
    {
        *errorPtr = 1;
        printf("Invalid Division By Zero");
        return -1;
    }
    if(operator == '+') return number1 + number2;
    if(operator == '-') return number1 - number2;
    if(operator == '*') return number1 * number2;
    if(operator == '/') return number1 / number2;
}

int evaluate(char expression[])
{
    int expLength = strlen(expression), errorPtr = 0;
    if(expLength == 0)
    {
        printf("Invalid Expression.\n");
        gOpTop++;
        return 0;
    }
    for(int index = 0; index < expLength; index++)
    {
        if(isspace(expression[index]))
        {
            continue;
        }

        if(isdigit(expression[index]))
        {
            int value = 0;
            while(isdigit(expression[index]))
            {
                value = value * 10 + (expression[index] - '0');
                index++;
            }
            pushNum(value);
            index--;
        }
        else if(expression[index] == '-' || expression[index] == '+' || 
                                            expression[index] == '/' || 
                                            expression[index] == '*')
        {
            while(gOpTop != -1 && precedance(expression[index]) <= precedance(gOpStack[gOpTop]))
            {
                int number1 = numPop();
                int number2 = numPop();
                char operator = opPop();
                int result = calculate(number1, number2, operator, &errorPtr);
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
            pushOp(expression[index]);
        }
        else
        {
            printf("'Error: Invalid expression.");
            return 0;
        }
    }
    while(gOpTop != -1)
    {
        int number1 = numPop();
        int number2 = numPop();
        char operator = opPop();
        int result = calculate(number1, number2, operator, &errorPtr);
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
    printf("Enter an Expression: ");
    scanf("%[^\n]", expression);
    int result = evaluate(expression);
    if(gNumTop != 0 || gOpTop != -1)
    {
        return 1;
    }
    printf("%d", result);
    return 0;
}
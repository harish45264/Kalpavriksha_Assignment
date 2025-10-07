#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int numStack[100];
int numTop = -1;

char opStack[100];
int opTop = -1;

void pushNum(int num){
    numStack[++numTop] = num;
}

void pushOp(char op){
    opStack[++opTop] = op;
}

int numPop(){
    return numStack[numTop--];
}

char opPop(){
    return opStack[opTop--];
}

int precedance(char c){
    if(c == '+' || c == '-'){
        return 1;
    }
    else if(c == '*' || c == '/'){
        return 2;
    }
    return -1;
}

int calculate(int num1, int num2, char op, int *err){
    if((num1 == 0 || num2 == 0) && op == '/'){
        *err = 1;
        printf("Invalid Division By Zero");
        return -1;
    }
    if(op == '+') return num1 + num2;
    if(op == '-') return num1 - num2;
    if(op == '*') return num1 * num2;
    if(op == '/') return num1 / num2;
}

int evaluate(char exp[]){
    int len = strlen(exp), err = 0;
    if(len == 0){
        printf("Invalid Expression.\n");
        return 0;
    }
    for(int i = 0; i < len; i++){
        if(isspace(exp[i])){
            continue;
        }

        if(isdigit(exp[i])){
            int val = 0;
            while(isdigit(exp[i])){
                val = val * 10 + (exp[i] - '0');
                i++;
            }
            pushNum(val);
            i--;
        }
        else if(exp[i] == '-' || exp[i] == '+' || exp[i] == '/' || exp[i] == '*'){
            while(opTop != -1 && precedance(exp[i]) <= precedance(opStack[opTop])){
                int num1 = numPop();
                int num2 = numPop();
                char op = opPop();
                int result = calculate(num1, num2, op, &err);
                if(err == 0){
                    pushNum(result);
                }
                else{
                    printf("Error: Division by zero.");
                    return 0;
                }
            }
            pushOp(exp[i]);
        }
        else{
            printf("'Error: Invalid expression.");
            return 0;
        }
    }
    while(opTop != -1){
        int num1 = numPop();
        int num2 = numPop();
        char op = opPop();
        int result = calculate(num1, num2, op, &err);
        if(result != -1){
            pushNum(result);
        }
    }
    return numStack[numTop];
}

int main(){
    char exp[100];
    scanf("%[^\n]", exp);
    int res = evaluate(exp);
    if(numTop != 0 || opTop != -1){
        return 1;
    }
    printf("%d", res);
    return 0;
}
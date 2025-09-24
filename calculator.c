#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int numStack[100];
int numTop = -1;

char opStack[100];
int opTop = -1

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

int eval(char exp[]){

}

int main(){
    char exp[100];
    scanf("%[^\n]", exp);
    printf("%s", exp);
}
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() 
{
	
    int number;
    scanf("%d", &number);
    int result = 0;
    while (number != 0)
    {
        int reminder = number % 10;
        number /= 10;
        result = result + reminder;
    }
    printf("%d", result);
    return 0;
}
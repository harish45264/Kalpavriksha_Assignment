#include <stdio.h>
#include <stdlib.h>

void reverseArray(int left, int right, int *arrayPtr);

void reverseArray (int left, int right, int* arrayPtr)
{
    while (left < right)
    {
        int tempVar = *(arrayPtr + left);
        *(arrayPtr + left) = *(arrayPtr + right);
        *(arrayPtr + right) = tempVar;
        right = right - 1;
        left = left + 1;
    }
}


int main()
{
    int number, *arrayPtr, index;
    scanf("%d", &number);
    arrayPtr = (int*) malloc(number * sizeof(int));
    for (index = 0; index < number; index++)
    {
        scanf("%d", arrayPtr + index);
    }

    reverseArray(0, number - 1, arrayPtr);

    for (index = 0; index < number; index++)
    {
        printf("%d ", *(arrayPtr + index));
    }
    return 0;
}
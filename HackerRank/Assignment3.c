#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int* rotateArray (int rotateCount, int arrayCount, int* arrayPtr, int* resultCount);
void reverseArray (int left, int right, int* resultPtr);

void reverseArray (int left, int right, int* resultPtr)
{
    while (left < right)
    {
        int tempVar = *(resultPtr + left);
        *(resultPtr + left) = *(resultPtr + right);
        *(resultPtr + right) = tempVar;
        right = right - 1;
        left = left + 1;
    }
}

int* rotateArray (int rotateCount, int arrayCount, int* arrayPtr, int* resultCount)
{
    int *resultPtr = malloc (arrayCount * sizeof(int));
    *resultCount = arrayCount;
    for(int index = 0; index < *resultCount; index++)
    {
        *(resultPtr + index) = *(arrayPtr + index);
    }
    rotateCount = rotateCount % arrayCount;
    reverseArray (0, rotateCount - 1, resultPtr);
    reverseArray (rotateCount, arrayCount - 1, resultPtr);
    reverseArray (0, arrayCount - 1, resultPtr);
    return resultPtr;
}
int main ()
{
    int arrayCount, rotateCount;
    scanf("%d %d", &arrayCount, &rotateCount);
    int *arrayPtr;
    arrayPtr = malloc (arrayCount * sizeof(int));
    for (int index = 0; index < arrayCount; index++)
    {
        scanf("%d", arrayPtr + index);
    }
    int resultCount;
    int *resultPtr = rotateArray (rotateCount, arrayCount, arrayPtr, &resultCount);
    for (int index = 0; index < resultCount; index++)
    {
        printf("%d ", *(resultPtr + index));
    }
}
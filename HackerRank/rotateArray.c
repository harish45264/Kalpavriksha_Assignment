#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int* rotateArray (int rotateCount, int arrayCount, int* arrayPtr, int* resultCount);
void reverseArray (int left, int right, int* resultPtr);
void displayOutput (int* resultPtr, int resultCount);
void getInput (int* arrayCount, int* rotateCount, int** arrayPtr);
void cleanUpMemory(int** arrayPtr, int** resultPtr);

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

void getInput (int* arrayCount, int* rotateCount, int** arrayPtr)
{
    printf("\nEnter arrayCount and rotateCount: ");
    scanf("%d %d", arrayCount, rotateCount);
    (*arrayPtr) = malloc (*arrayCount * sizeof(int));
    printf("Enter array elements: \n");
    for (int index = 0; index < *arrayCount; index++)
    {
        scanf("%d", (*arrayPtr) + index);
    }
}

void displayOutput (int* resultPtr, int resultCount)
{
    for (int index = 0; index < resultCount; index++)
    {
        printf("%d ", *(resultPtr + index));
    }
}
void cleanUpMemory(int** arrayPtr, int** resultPtr)
{
    free((*arrayPtr));
    free((*resultPtr));
}
int main ()
{
    int arrayCount, rotateCount, resultCount;
    int *arrayPtr;
    getInput(&arrayCount, &rotateCount, &arrayPtr);
    int *resultPtr = rotateArray (rotateCount, arrayCount, arrayPtr, &resultCount);
    displayOutput(resultPtr, resultCount);
    cleanUpMemory(&arrayPtr, &resultPtr);
}
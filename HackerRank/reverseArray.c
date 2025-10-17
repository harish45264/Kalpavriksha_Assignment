#include <stdio.h>
#include <stdlib.h>

void reverseArray(int left, int right, int *arrayPtr);
void getInput (int* arraySize, int** arrayPtr);
void displayOutput (int arraySize, int* arrayPtr);
void cleanUpMemory (int** arrayPtr);

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

void getInput (int* arraySize, int** arrayPtr)
{
    printf("\nEnter Number of elements: ");
    scanf("%d", arraySize);
    (*arrayPtr) = (int*) malloc((*arraySize) * sizeof(int));
    printf("\nEnter array elements: \n");
    for (int rowPos = 0; rowPos < *arraySize; rowPos++)
    {
        scanf("%d", (*arrayPtr) + rowPos);
    }
}

void displayOutput (int arraySize, int* arrayPtr)
{
    for (int rowPos = 0; rowPos < arraySize; rowPos++)
    {
        printf("%d ", *(arrayPtr + rowPos));
    }
}

void cleanUpMemory (int** arrayPtr)
{
    free((*arrayPtr));
}

int main()
{
    int arraySize, *arrayPtr;
    getInput(&arraySize, &arrayPtr);
    reverseArray(0, arraySize - 1, arrayPtr);
    displayOutput(arraySize, arrayPtr);
    return 0;
}
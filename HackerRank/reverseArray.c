#include <stdio.h>
#include <stdlib.h>

void reverseArray(int left, int right, int *arrayPtr);
void getInput (int* number, int** arrayPtr);
void displayOutput (int number, int* arrayPtr);
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

void getInput (int* number, int** arrayPtr)
{
    printf("\nEnter Number of elements: ");
    scanf("%d", number);
    (*arrayPtr) = (int*) malloc((*number) * sizeof(int));
    printf("\nEnter array elements: \n");
    for (int index = 0; index < *number; index++)
    {
        scanf("%d", (*arrayPtr) + index);
    }
}

void displayOutput (int number, int* arrayPtr)
{
    for (int index = 0; index < number; index++)
    {
        printf("%d ", *(arrayPtr + index));
    }
}

void cleanUpMemory (int** arrayPtr)
{
    free((*arrayPtr));
}

int main()
{
    int number, *arrayPtr;
    getInput(&number, &arrayPtr);
    reverseArray(0, number - 1, arrayPtr);
    displayOutput(number, arrayPtr);
    return 0;
}
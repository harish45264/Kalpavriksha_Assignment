#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum
{
    RESULT_SUCCESS,
    MEMORY_ERROR
} resultCode;

resultCode getUserInput (void);
resultCode allocatememory (int*** matrixPtr);
resultCode assignRandomValues (int*** matrixPtr);
void displayMatrix (int*** matrixPtr);
resultCode reverseArray (int left, int right, int* currRowPtr);
resultCode rotateMatrix (int*** matrixPtr);
resultCode applySmoothening (int*** matrixPtr);
void cleanUpData (int*** matrixPtr);
void printResultCode (resultCode result);

const int maxRange = 256;
static int gSize;

resultCode getUserInput ()
{
    printf("Enter matrixPtr size (2 - 10): ");
    scanf("%d", &gSize);
    return RESULT_SUCCESS;
}

void printResultCode (resultCode result)
{
    switch (result)
    {
    case RESULT_SUCCESS:
        printf("\nOperation Successfull!\n");
        break;
    case MEMORY_ERROR:
        printf("\nError while allocating Memory!\n");
        exit(0);
    }
}

resultCode allocateMemory (int*** matrixPtr)
{
    (*matrixPtr) = calloc(gSize, sizeof(int*));
    if ((*matrixPtr) == NULL)
    {
        return MEMORY_ERROR;
    }
    for (int index = 0; index < gSize; index++)
    {
        *((*matrixPtr) + index) = calloc(gSize, sizeof(int));
        if (*((*matrixPtr) + index) == NULL)
        {
            free((*matrixPtr));
            return MEMORY_ERROR;
        }
    }
    return RESULT_SUCCESS;
}

void cleanUpData (int*** matrixPtr)
{
    free((*matrixPtr));
    (*matrixPtr) = NULL;
}

resultCode assignRandomValues (int*** matrixPtr)
{
    srand(time(0));
    int** currMatrixPtr = (*matrixPtr);
    if (currMatrixPtr == NULL)
    {
        return MEMORY_ERROR;
    }
    for (int rIndex = 0; rIndex < gSize; rIndex++)
    {
        for (int cIndex = 0; cIndex < gSize; cIndex++)
        {
            *(*(currMatrixPtr + rIndex) + cIndex) = rand() % maxRange;
        }
    }
    return RESULT_SUCCESS;
}

void displayMatrix (int*** matrixPtr)
{
    int** currMatrixPtr = (*matrixPtr);
    for (int rIndex = 0; rIndex < gSize; rIndex++)
    {
        for (int cIndex = 0; cIndex < gSize; cIndex++)
        {
            printf("%d ", *(*(currMatrixPtr + rIndex) + cIndex));
        }
        printf("\n");
    }
}

resultCode reverseArray (int left, int right, int* currRowPtr)
{
    if (currRowPtr == NULL)
    {
        return MEMORY_ERROR;
    }
    while (left < right)
    {
        int tempVariable = *(currRowPtr + left);
        *(currRowPtr + left) = *(currRowPtr + right);
        *(currRowPtr + right) = tempVariable;
        right = right - 1;
        left = left + 1;
    }
    return RESULT_SUCCESS;
}

resultCode rotateMatrix (int*** matrixPtr)
{
    int** currMatrixPtr = (*matrixPtr);
    if (currMatrixPtr == NULL)
    {
        return MEMORY_ERROR;
    }
    for (int rIndex = 0; rIndex < gSize; rIndex++)
    {
        for (int cIndex = rIndex + 1; cIndex < gSize; cIndex++)
        {
            int tmpVariable = *(*(currMatrixPtr + rIndex) + cIndex);
            *(*(currMatrixPtr + rIndex) + cIndex) = *(*(currMatrixPtr + cIndex) + rIndex);
            *(*(currMatrixPtr + cIndex) + rIndex) = tmpVariable;
        }
        reverseArray(0, gSize - 1, *(currMatrixPtr + rIndex));
    }
    return RESULT_SUCCESS;
}

resultCode applySmoothening (int*** matrixPtr)
{
    int* prevTempArray;
    int** currMatrixPtr = (*matrixPtr);
    if (currMatrixPtr == NULL)
    {
        return MEMORY_ERROR;
    }
    for (int rIndex = 0; rIndex < gSize; rIndex++)
    {
        int* tempArray = malloc(gSize * sizeof(int));
        if (tempArray == NULL)
        {
            return MEMORY_ERROR;
        }
        for (int cIndex = 0; cIndex < gSize; cIndex++)
        {
            int count = 0, sum = 0;
            for (int sIndex1 = rIndex - 1; sIndex1 <= rIndex + 1; sIndex1++)
            {
                for (int sIndex2 = cIndex - 1; sIndex2 <= cIndex + 1; sIndex2++)
                {
                    if(sIndex1 >= 0 && sIndex1 < gSize && sIndex2 >= 0 && sIndex2 < gSize)
                    {
                        sum += *(*(currMatrixPtr + sIndex1) + sIndex2);
                        count += 1;
                    }
                }
            }
            sum = sum / count;
            *(tempArray + cIndex) = sum;
        }
        if (rIndex > 0)
        {
            for (int curIndex = 0; curIndex < gSize; curIndex++)
            {
                *(*(currMatrixPtr + rIndex - 1) + curIndex) = *(prevTempArray + curIndex);
            }
        }
        prevTempArray = tempArray;
    }
    for (int curIndex = 0; curIndex < gSize; curIndex++)
    {
        *(*(currMatrixPtr + gSize - 1) + curIndex) = *(prevTempArray + curIndex);
    }
    return RESULT_SUCCESS;
}

int main ()
{
    int **matrixPtr;
    resultCode result;
    result = getUserInput();
    printResultCode(result);
    result = allocateMemory(&matrixPtr);
    printResultCode(result);
    result = assignRandomValues(&matrixPtr);
    printResultCode(result);
    printf("\nOriginal:\n\n");
    displayMatrix(&matrixPtr);
    result = rotateMatrix(&matrixPtr);
    printResultCode(result);
    printf("\nRotated: \n\n");
    displayMatrix(&matrixPtr);
    result = applySmoothening(&matrixPtr);
    printResultCode(result);
    printf("\nFinal Output: \n\n");
    displayMatrix(&matrixPtr);
    cleanUpData(&matrixPtr);
}
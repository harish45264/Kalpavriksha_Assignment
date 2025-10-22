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

const int gMaxRange = 256;
static int gMatrixSize;

resultCode getUserInput ()
{
    printf("Enter matrixPtr size (2 - 10): ");
    scanf("%d", &gMatrixSize);
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
    (*matrixPtr) = calloc(gMatrixSize, sizeof(int*));
    if ((*matrixPtr) == NULL)
    {
        return MEMORY_ERROR;
    }
    for (int rowPos = 0; rowPos < gMatrixSize; rowPos++)
    {
        *((*matrixPtr) + rowPos) = calloc(gMatrixSize, sizeof(int));
        if (*((*matrixPtr) + rowPos) == NULL)
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
    for (int rowPos = 0; rowPos < gMatrixSize; rowPos++)
    {
        for (int columnPos = 0; columnPos < gMatrixSize; columnPos++)
        {
            *(*(currMatrixPtr + rowPos) + columnPos) = rand() % gMaxRange;
        }
    }
    return RESULT_SUCCESS;
}

void displayMatrix (int*** matrixPtr)
{
    int** currMatrixPtr = (*matrixPtr);
    for (int rowPos = 0; rowPos < gMatrixSize; rowPos++)
    {
        for (int columnPos = 0; columnPos < gMatrixSize; columnPos++)
        {
            printf("%d ", *(*(currMatrixPtr + rowPos) + columnPos));
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
    for (int rowPos = 0; rowPos < gMatrixSize; rowPos++)
    {
        for (int columnPos = rowPos + 1; columnPos < gMatrixSize; columnPos++)
        {
            int tmpVariable = *(*(currMatrixPtr + rowPos) + columnPos);
            *(*(currMatrixPtr + rowPos) + columnPos) = *(*(currMatrixPtr + columnPos) + rowPos);
            *(*(currMatrixPtr + columnPos) + rowPos) = tmpVariable;
        }
        reverseArray(0, gMatrixSize - 1, *(currMatrixPtr + rowPos));
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
    for (int rowPos = 0; rowPos < gMatrixSize; rowPos++)
    {
        int* tempArray = malloc(gMatrixSize * sizeof(int));
        if (tempArray == NULL)
        {
            return MEMORY_ERROR;
        }
        for (int columnPos = 0; columnPos < gMatrixSize; columnPos++)
        {
            int count = 0, sum = 0;
            for (int deltaRow = rowPos - 1; deltaRow <= rowPos + 1; deltaRow++)
            {
                for (int deltaColumn = columnPos - 1; deltaColumn <= columnPos + 1; deltaColumn++)
                {
                    if(deltaRow >= 0 && deltaRow < gMatrixSize && deltaColumn >= 0 && deltaColumn < gMatrixSize)
                    {
                        sum += *(*(currMatrixPtr + deltaRow) + deltaColumn);
                        count += 1;
                    }
                }
            }
            sum = sum / count;
            *(tempArray + columnPos) = sum;
        }
        if (rowPos > 0)
        {
            for (int curIndex = 0; curIndex < gMatrixSize; curIndex++)
            {
                *(*(currMatrixPtr + rowPos - 1) + curIndex) = *(prevTempArray + curIndex);
            }
        }
        prevTempArray = tempArray;
    }
    for (int curIndex = 0; curIndex < gMatrixSize; curIndex++)
    {
        *(*(currMatrixPtr + gMatrixSize - 1) + curIndex) = *(prevTempArray + curIndex);
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
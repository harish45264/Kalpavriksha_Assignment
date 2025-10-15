#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int* findMatching (int stringCount, int queryCount, char*** stringPtr, char*** queryPtr);
void getInput(int *stringCount, int *queryCount, char*** stringPtr, char*** queryPtr);
int findLength (char* currRow);
bool stringEquals (int qLength, int sLength, char* qRow, char* sRow);
void displayOutput(int queryCount, int* resultArr);
void cleanUpMemory (char*** stringPtr, char*** queryPtr, int* resultArr);

void displayOutput(int queryCount, int* resultArr)
{
    printf("Output:\n");
    for(int index = 0; index < queryCount; index++)
    {
        printf("%d\n", *(resultArr + index));
    }
}

void cleanUpMemory (char*** stringPtr, char*** queryPtr, int* resultArr)
{
    free(stringPtr);
    free(queryPtr);
    free(resultArr);
}

int findLength (char* currRow)
{
    int length = 0;
    while (*(currRow + length) != '\0')
    {
        length++;
    }
    return length;
}

bool stringEquals (int qLength, int sLength, char* qRow, char* sRow)
{
    if (qLength != sLength)
    {
        return false;
    }
    while (qLength >= 0)
    {
        if (*(qRow + qLength) != *(sRow + qLength))
        {
            return false;
        }
        qLength -= 1;
    }
    return true;
}

int* findMatching (int stringCount, int queryCount, char*** stringPtr, char*** queryPtr)
{
    int* resultArr = malloc(queryCount * sizeof(int));
    if (resultArr == NULL)
        {
            printf("Error while allocating memory!");
            free(resultArr);
            return resultArr;
        }
    for (int qIndex = 0; qIndex < queryCount; qIndex++)
    {
        char* qRow = *((*queryPtr) + qIndex);
        int qLength = findLength(qRow), count = 0;
        for (int sIndex = 0; sIndex < stringCount; sIndex++)
        {
            char* sRow = *((*stringPtr) + sIndex);
            int sLength = findLength(sRow);
            if(stringEquals(qLength - 1, sLength - 1, qRow, sRow))
            {
                count++;
            }
        }
        *(resultArr + qIndex) = count;
    }
    return resultArr;
}

void getInput(int *stringCount, int *queryCount, char*** stringPtr, char*** queryPtr)
{
    printf("\nEnter string count: ");
    scanf("%d", stringCount);
    (*stringPtr) = malloc((*stringCount) * sizeof(char*));
    if((*stringPtr) == NULL)
    {
        printf("Error while allocating memory!");
        free((*stringPtr));
        return;
    }
    printf("\nEnter strings:\n");
    for (int index = 0; index < *stringCount; index++)
    {
        *((*stringPtr) + index) = malloc(100 * sizeof(char));
        if (*((*stringPtr) + index) == NULL)
        {
            printf("Error while allocating memory!");
            free(*((*stringPtr) + index));
            return;
        }
        scanf("%s", *((*stringPtr) + index));
    }
    printf("\nEnter query count: ");
    scanf("%d", queryCount);
    (*queryPtr) = malloc((*queryCount) * sizeof(char*));
    if((*queryPtr) == NULL)
    {
        printf("Error while allocating memory!");
        free((*queryPtr));
        return;
    }
    printf("\nEnter queries:\n");
    for (int index = 0; index < *queryCount; index++)
    {
        *((*queryPtr) + index) = malloc(100 * sizeof(char));
        if (*((*queryPtr) + index) == NULL)
        {
            printf("Error while allocating memory!");
            free(*((*queryPtr) + index));
            return;
        }
        scanf("%s", *((*queryPtr) + index));
    }
}

int main ()
{
    int stringCount, queryCount;
    char **stringPtr;
    char **queryPtr;
    getInput(&stringCount, &queryCount, &stringPtr, &queryPtr);
    int* resultArr = findMatching(stringCount, queryCount, &stringPtr, &queryPtr);
    displayOutput(queryCount, resultArr);
    cleanUpMemory(&stringPtr, &queryPtr, resultArr);
}
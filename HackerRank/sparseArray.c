#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int* findMatching (int stringCount, int queryCount, char*** stringPtr, char*** queryPtr);
void getInput(int *stringCount, int *queryCount, char*** stringPtr, char*** queryPtr);
int findLength (char* currRow);
bool stringEquals (int queryLength, int strLength, char* queryRow, char* strRow);
void displayOutput(int queryCount, int* resultArr);
void cleanUpMemory (char*** stringPtr, char*** queryPtr, int* resultArr);

void displayOutput(int queryCount, int* resultArr)
{
    printf("Output:\n");
    for(int rowPos = 0; rowPos < queryCount; rowPos++)
    {
        printf("%d\n", *(resultArr + rowPos));
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

bool stringEquals (int queryLength, int strLength, char* queryRow, char* strRow)
{
    if (queryLength != strLength)
    {
        return false;
    }
    while (queryLength >= 0)
    {
        if (*(queryRow + queryLength) != *(strRow + queryLength))
        {
            return false;
        }
        queryLength -= 1;
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
    for (int queryRowPos = 0; queryRowPos < queryCount; queryRowPos++)
    {
        char* queryRow = *((*queryPtr) + queryRowPos);
        int queryLength = findLength(queryRow), count = 0;
        for (int strRowPos = 0; strRowPos < stringCount; strRowPos++)
        {
            char* strRow = *((*stringPtr) + strRowPos);
            int strLength = findLength(strRow);
            if(stringEquals(queryLength - 1, strLength - 1, queryRow, strRow))
            {
                count++;
            }
        }
        *(resultArr + queryRowPos) = count;
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
    for (int rowPos = 0; rowPos < *stringCount; rowPos++)
    {
        *((*stringPtr) + rowPos) = malloc(100 * sizeof(char));
        if (*((*stringPtr) + rowPos) == NULL)
        {
            printf("Error while allocating memory!");
            free(*((*stringPtr) + rowPos));
            return;
        }
        scanf("%s", *((*stringPtr) + rowPos));
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
    for (int rowPos = 0; rowPos < *queryCount; rowPos++)
    {
        *((*queryPtr) + rowPos) = malloc(100 * sizeof(char));
        if (*((*queryPtr) + rowPos) == NULL)
        {
            printf("Error while allocating memory!");
            free(*((*queryPtr) + rowPos));
            return;
        }
        scanf("%s", *((*queryPtr) + rowPos));
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
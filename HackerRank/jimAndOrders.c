#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void getInput (int* rowCount, int* colCount, int*** orders);
int* jimOrders (int rowCount, int colCount, int*** orders, int* resultCount);
void displayOutput (int resultCount, int* customers);
void cleanUpMemory (int*** orders, int** customers);

int* jimOrders (int rowCount, int colCount, int*** orders, int* resultCount)
{
    int* customers = malloc (rowCount * sizeof(int));
    int* serveTime= malloc (rowCount * sizeof(int));
    *resultCount = rowCount;
    for (int index = 0; index < rowCount; index++)
    {
        *(customers + index) = index + 1;
        *(serveTime + index) = *(*((*orders) + index) + 0) + *(*((*orders) + index) + 1);
    }

    for (int index1 = 0; index1 < rowCount; index1++)
    {
        for (int index2 = 0; index2 < rowCount - index1 - 1; index2++)
        {
            if (*(serveTime + index2) > *(serveTime + index2 + 1))
            {
                int tempVar = *(serveTime + index2);
                *(serveTime + index2) = *(serveTime + index2 + 1);
                *(serveTime + index2 + 1) = tempVar;
                
                tempVar = *(customers + index2);
                *(customers + index2) = *(customers + index2 + 1);
                *(customers + index2 + 1) = tempVar;
            }
        }
    }
    return customers;
}

void cleanUpMemory (int*** orders, int** customers)
{
    free((*orders));
    free((*customers));
}

void displayOutput (int resultCount, int* customers)
{
    for(int index = 0; index < resultCount; index++)
    {
        printf("%d ", *(customers + index));
    }
}

void getInput (int* rowCount, int* colCount, int*** orders)
{
    printf("Enter Number: ");
    scanf("%d", rowCount);
    (*orders) = malloc (*rowCount * sizeof(int*));
    for (int index = 0; index < *rowCount; index++)
    {
        *((*orders) + index) = malloc (2 * sizeof(int));
    }
    for (int index = 0; index < *rowCount; index++)
    {
        scanf("%d %d", (*((*orders) + index) + 0), (*((*orders) + index) + 1));
    }
}

int main ()
{
    int rowCount, colCount = 2, resultCount;
    int **orders;
    getInput(&rowCount, &colCount, &orders);
    int* customers = jimOrders (rowCount, colCount, &orders, &resultCount);
    displayOutput (resultCount, customers);
    cleanUpMemory (&orders, &customers);
}
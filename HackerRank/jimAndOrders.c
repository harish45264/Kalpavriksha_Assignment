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
    for (int rowPos = 0; rowPos < rowCount; rowPos++)
    {
        *(customers + rowPos) = rowPos + 1;
        *(serveTime + rowPos) = *(*((*orders) + rowPos) + 0) + *(*((*orders) + rowPos) + 1);
    }

    for (int rowPos = 0; rowPos < rowCount; rowPos++)
    {
        for (int columnPos = 0; columnPos < rowCount - rowPos - 1; columnPos++)
        {
            if (*(serveTime + columnPos) > *(serveTime + columnPos + 1))
            {
                int tempVar = *(serveTime + columnPos);
                *(serveTime + columnPos) = *(serveTime + columnPos + 1);
                *(serveTime + columnPos + 1) = tempVar;
                
                tempVar = *(customers + columnPos);
                *(customers + columnPos) = *(customers + columnPos + 1);
                *(customers + columnPos + 1) = tempVar;
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
    for(int rowPos = 0; rowPos < resultCount; rowPos++)
    {
        printf("%d ", *(customers + rowPos));
    }
}

void getInput (int* rowCount, int* colCount, int*** orders)
{
    printf("Enter Number: ");
    scanf("%d", rowCount);
    (*orders) = malloc (*rowCount * sizeof(int*));
    for (int rowPos = 0; rowPos < *rowCount; rowPos++)
    {
        *((*orders) + rowPos) = malloc (2 * sizeof(int));
    }
    for (int rowPos = 0; rowPos < *rowCount; rowPos++)
    {
        scanf("%d %d", (*((*orders) + rowPos) + 0), (*((*orders) + rowPos) + 1));
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
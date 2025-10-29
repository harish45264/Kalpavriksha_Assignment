#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int num1, int num2);
int surfaceArea(int A_rows, int A_columns, int** A);
int surfaceArea(int A_rows, int A_columns, int** A);

int min(int num1, int num2){
    return (num1 < num2) ? num1 : num2;
}

int visibleFaces(int A_rows, int A_columns, int row, int col, int** A){
    int down = *(*(A + row) + col), up = *(*(A + row) + col), right = *(*(A + row) + col), left = *(*(A + row) + col);
    if((row + 1) < A_rows){
        down = *(*(A + row) + col) - min(*(*(A + row + 1) + col), *(*(A + row) + col));
    }
    if((row - 1) >= 0){
        up = *(*(A + row) + col) - min(*(*(A + row - 1) + col), *(*(A + row) + col));
    }
    if((col + 1) < A_columns){
        right = *(*(A + row) + col) - min(*(*(A + row) + col + 1), *(*(A + row) + col));
    }
    if((col - 1) >= 0){
        left = *(*(A + row) + col) - min(*(*(A + row) + col - 1), *(*(A + row) + col));
    }
    return left + right + down + up;
}

int surfaceArea(int A_rows, int A_columns, int** A) {
    int total = 0;
    for (int row = 0; row < A_rows; row++){
        for(int col = 0; col < A_columns; col++){
            int currH = *(*(A + row) + col);
            int currS = (currH * 2) - ((currH - 1) * 2);
            int visible = visibleFaces(A_rows, A_columns, row, col, A);
            total += (visible + currS);
        }
    }
    return total;
}

int main()
{
    int A_columns, A_rows;
    printf("Enter rows and columns: ");
    scanf("%d %d", &A_rows, &A_columns);
    int** A = malloc(A_rows * sizeof(int*));
    for (int row = 0; row < A_rows; row++)
    {
        *(A + row) = calloc(A_columns, sizeof(int));
    }
    printf("Enter the elements:\n");
    for (int row = 0; row < A_rows; row++){
        for(int col = 0; col < A_columns; col++){
            scanf("%d", (*(A + row) + col));
        }
    }
    int result = surfaceArea (A_rows, A_columns, A);
    printf("%d", result);
    for (int row = 0; row < A_rows; row++)
    {
        free(*(A + row));
    }
    free(A);
    return 0;
}
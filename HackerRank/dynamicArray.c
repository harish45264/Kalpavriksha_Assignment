#include <stdio.h>
#include <stdlib.h>

int* total_number_of_books;
int** total_number_of_pages;

int main()
{
    int total_number_of_shelves;
    scanf("%d", &total_number_of_shelves);
    
    int total_number_of_queries;
    scanf("%d", &total_number_of_queries);
    
    total_number_of_books = calloc(total_number_of_shelves, sizeof(int));
    total_number_of_pages = calloc(total_number_of_shelves, sizeof(int*));
    
    while (total_number_of_queries--) {
        int type_of_query;
        scanf("%d", &type_of_query);
        
        if (type_of_query == 1) {
            int shelfIndex, pages;
            scanf("%d %d", &shelfIndex, &pages);
            int cur = total_number_of_books[shelfIndex];
            int *tmp = realloc(total_number_of_pages[shelfIndex], (cur + 1)*sizeof(int));
            
            total_number_of_pages[shelfIndex] = tmp;
            total_number_of_pages[shelfIndex][cur] = pages;
            total_number_of_books[shelfIndex]++;

        } else if (type_of_query == 2) {
            int shelfIndex, bookIndex;
            scanf("%d %d", &shelfIndex, &bookIndex);
            printf("%d\n", *(*(total_number_of_pages + shelfIndex) + bookIndex));
        } else {
            int shelfIndex;
            scanf("%d", &shelfIndex);
            printf("%d\n", *(total_number_of_books + shelfIndex));
        }
    }

    if (total_number_of_books) {
        free(total_number_of_books);
    }
    
    for (int index = 0; index < total_number_of_shelves; index++) {
        if (*(total_number_of_pages + index)) {
            free(*(total_number_of_pages + index));
        }
    }
    
    if (total_number_of_pages) {
        free(total_number_of_pages);
    }
    
    return 0;
}
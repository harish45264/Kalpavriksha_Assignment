#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int productId;
    char* productName;
    float productPrice;
    int productQuantity;
} product;

typedef enum
{
    PRODUCT_ADDED,
    PRODUCT_UPDATED,
    PRODUCT_DELETED,
    PRODUCT_FOUND,
    NOT_FOUND,
    MEMORY_ERROR
} resultCode;

//Global Variables
static int isRunning = 1;
static int numberOfProds = 0;
product *productArrayPtr;
const int structSize = sizeof(product);

//Function Definitions
resultCode addInitialProduct ();
void printResult (resultCode result);
void displayAllProducts ();
void displayMenu ();
void displayProduct (product* currProduct);
resultCode addProduct ();
resultCode updateQuantity ();
resultCode searchById ();
resultCode searchByName ();
bool stringCompare (char* keyName, char* productName);
int findLength (char* word);
resultCode searchByPriceRange ();
resultCode deleteById ();


int findLength (char* word)
{
    int length = 0;
    while (*(word + length) != '\0')
    {
        length += 1;
    }
    return length;
}

bool stringCompare (char* keyName, char* productName)
{
    int keyNameLength = findLength (keyName);
    int prodNameLength = findLength (productName);
    for (int prodPosition = 0; prodPosition <= prodNameLength - keyNameLength; prodPosition++)
    {
        int keyIndex;
        for (keyIndex = 0; keyIndex < keyNameLength; keyIndex++)
        {
            if (*(productName + prodPosition + keyIndex) != *(keyName + keyIndex))
            {
                break;
            }
        }
        if (keyIndex == keyNameLength)
        {
            return true;
        }
    }
    return false;
}

resultCode searchByName ()
{
    char* keyName;
    bool isFound = false;
    keyName = malloc(100 * sizeof(char));
    printf("\nEnter name to search: ");
    scanf("%99s", keyName);
    for (int iterator = 0; iterator < numberOfProds; iterator++)
    {
        if (stringCompare(keyName, (productArrayPtr + iterator)->productName))
        {
            isFound = true;
            displayProduct (productArrayPtr + iterator);
        }
    }
    if (isFound) return PRODUCT_FOUND;
    return NOT_FOUND;
}

resultCode searchByPriceRange ()
{
    int minPrice, maxPrice;
    bool isFound = false;
    printf("Enter minimum Price: ");
    scanf("%d", &minPrice);
    printf("Enter maximum Price: ");
    scanf("%d", &maxPrice);
    printf("Products in price range: ");
    for (int iterator = 0; iterator < numberOfProds; iterator++)
    {
        if ((productArrayPtr + iterator)->productPrice >= minPrice && (productArrayPtr + iterator)->productPrice <= maxPrice)
        {
            isFound = true;
            displayProduct((productArrayPtr + iterator));
        }
    }
    if (isFound) return PRODUCT_FOUND;
    return NOT_FOUND;
}

//Binary search to find product
resultCode searchById ()
{
    int keyId;
    printf("\nEnter Product ID to search: ");
    scanf("%d", &keyId);
    for (int iterator = 0; iterator < numberOfProds; iterator++)
    {
        if ((productArrayPtr + iterator)->productId == keyId)
        {
            displayProduct ((productArrayPtr + iterator));
            return PRODUCT_FOUND;
        }
    }
    return NOT_FOUND;
}

resultCode deleteById ()
{
    int keyId, prodPosition = -1;
    bool isFound = false;
    printf("Enter product ID to delete: ");
    scanf("%d", &keyId);
    for (int iterator = 0; iterator < numberOfProds; iterator++)
    {
        if ((productArrayPtr + iterator)->productId == keyId)
        {
            isFound = true;
            prodPosition = iterator;
            break;
        }
    }
    if (!isFound) return NOT_FOUND;
    for (int iterator = prodPosition; iterator < numberOfProds - 1; iterator++)
    {
        *(productArrayPtr + iterator) = *(productArrayPtr + iterator + 1);
    }
    numberOfProds -= 1;
    return PRODUCT_DELETED;
}

//Display single product
void displayProduct (product* currProduct)
{
    printf("\nProduct ID: %d | Name: %s | Price: %.2f | Quantity: %d",(*currProduct).productId, 
                                                                      (*currProduct).productName, 
                                                                      (*currProduct).productPrice, 
                                                                      (*currProduct).productQuantity);
}

//Func to update quatity using product ID
resultCode updateQuantity ()
{
    if (productArrayPtr == NULL)
    {
        return MEMORY_ERROR;
    }
    int keyId;
    bool isFound = false;
    printf("\nEnter Product ID to update quantity: ");
    scanf("%d", &keyId);
    product* currProduct;
    for (int iterator = 0; iterator < numberOfProds; iterator++)
    {
        if ((productArrayPtr + iterator)->productId == keyId)
        {
            isFound = true;
            currProduct = (productArrayPtr + iterator);
            break;
        }
    }
    if (!isFound)
    {
        return NOT_FOUND;
    }
    printf("\nEnter new Quantity: ");
    scanf("%d", &(currProduct)->productQuantity);
    return PRODUCT_UPDATED;
}

//Func to add new product
resultCode addProduct ()
{
    numberOfProds;
    if (productArrayPtr == NULL)
    {
        return MEMORY_ERROR;
    }
    productArrayPtr = realloc (productArrayPtr, (numberOfProds + 1) * structSize);
    printf("\nEnter new product details: ");
    printf("\nProduct ID: ");
    scanf("%d", &((productArrayPtr + numberOfProds)->productId));
    ((productArrayPtr + numberOfProds)->productName) = malloc (100 * sizeof(char));
    printf("\nProduct Name: ");
    scanf("%99s", ((productArrayPtr + numberOfProds)->productName));
    printf("\nProduct Price: ");
    scanf("%f", &((productArrayPtr + numberOfProds)->productPrice));
    printf("\nProduct Quantity: ");
    scanf("%d", &((productArrayPtr + numberOfProds)->productQuantity));
    numberOfProds += 1;
    return PRODUCT_ADDED;    
}

//Display App Menu
void displayMenu ()
{
    printf("\n========= INVENTORY MENU =========");
    printf("\n1. Add New Product"); 
    printf("\n2. View All Products");
    printf("\n3. Update Quantity"); 
    printf("\n4. Search Product by ID"); 
    printf("\n5. Search Product by Name");
    printf("\n6. Search Product by Price Range"); 
    printf("\n7. Delete Product"); 
    printf("\n8. Exit"); 
    printf("\nEnter your choice: ");
}

//Display all products
void displayAllProducts ()
{
    for (int iterator = 0; iterator < numberOfProds; iterator++)
    {
        printf("\nProduct ID: %d | Name: %s | Price: %.2f | Quantity: %d",(productArrayPtr + iterator)->productId, 
                                                                          (productArrayPtr + iterator)->productName, 
                                                                          (productArrayPtr + iterator)->productPrice, 
                                                                          (productArrayPtr + iterator)->productQuantity);
    }
}

//Print Results Based on resultCode
void printResult (resultCode result)
{
    if (result == PRODUCT_ADDED)
    {
        printf("\nProduct Added Successfully!");
    }
    else if (result == PRODUCT_UPDATED)
    {
        printf("\nQuantity Updated Successfully!");
    }
    else if (result == PRODUCT_DELETED)
    {
        printf("\nProduct Deleted Successfully!");
    }
    else if (result == NOT_FOUND)
    {
        printf("Product Not Found!");
    }
    else if (result == PRODUCT_FOUND)
    {
        printf("Products Found!");
    }
    else if (result == MEMORY_ERROR)
    {
        printf("Error while allocating memory!");
    }
}

//Func to add initial products 
resultCode addInitialProduct ()
{
    printf("\nEnter initial number of products: ");
    scanf("%d", &numberOfProds);
    productArrayPtr = malloc(numberOfProds * structSize);
    if (productArrayPtr == NULL)
    {
        return MEMORY_ERROR;
    }
    for (int iterator = 0; iterator < numberOfProds; iterator++)
    {
        printf("\nEnter details for product %d:", iterator + 1);
        printf("\nProduct ID: ");
        scanf("%d", &((productArrayPtr + iterator)->productId));
        ((productArrayPtr + iterator)->productName) = malloc (100 * sizeof(char));
        printf("\nProduct Name: ");
        scanf("%99s", ((productArrayPtr + iterator)->productName));
        printf("\nProduct Price: ");
        scanf("%f", &((productArrayPtr + iterator)->productPrice));
        printf("\nProduct Quantity: ");
        scanf("%d", &((productArrayPtr + iterator)->productQuantity));
    }
    return PRODUCT_ADDED;
}

void freeUpMemory () 
{
    free (productArrayPtr);
    printf("Memory released successfully!, Exiting program...");
}

int main ()
{
    resultCode result;
    result = addInitialProduct ();
    printResult (result);
    while (isRunning)
    {
        displayMenu();
        int choice;
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                result = addProduct ();
                printResult (result);
                break;
            case 2:
                displayAllProducts ();
                break;
            case 3:
                result = updateQuantity ();
                printResult (result);
                break;
            case 4:
                result = searchById ();
                printResult (result);
                break;
            case 5:
                result = searchByName ();
                printResult (result);
                break;
            case 6:
                result = searchByPriceRange ();
                printResult (result);
                break;
            case 7:
                result = deleteById ();
                printResult (result);
                break;
            case 8:
                isRunning = 0;
                freeUpMemory ();
                break;
        }
    }
}
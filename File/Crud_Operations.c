#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "Users.DAT"

void displayMenu()
{
    printf("\n1. Add User Data\n");
    printf("2. Show All User Data\n");
    printf("3. Update User Data\n");
    printf("4. Delete User Data\n");
    printf("5. Exit\n");
    printf("Enter your Choice: ");
}

struct User
{
    int userId;
    char userName[100];
    int userAge;
};

struct User user;

int gUniqueId = 0;


void assignId()
{
    FILE *filePtr = fopen(FILE_NAME, "rb");
    if(filePtr == NULL)
    {
        printf("\nError While Opening File! OR No Such File Found");
        return;
    }
    while(fread(&user, sizeof(user), 1, filePtr) == 1)
    {
        gUniqueId = user.userId;
    }
    gUniqueId++;
    fclose(filePtr);
}

void addUser()
{
    FILE *filePtr = fopen(FILE_NAME,"ab");
    if(filePtr == NULL)
    {
        printf("\nError while opening file!");
        return;
    }
    int result;
    do
    {
        printf("\nEnter Name: ");
        result = scanf("%[^\n]", user.userName);
        getchar();
        if(result == 0)
        {
            printf("Invalid name! Please enter a valid name.\n");
        }
    }while(result == 0);
    do
    {
        printf("\nEnter Age: ");
        scanf("%d", &user.userAge);
        getchar();
        if(user.userAge <= 0 || user.userAge >= 100)
        {
            printf("Invalid age! Please enter a number between 1 and 99.\n");
        }
    }while(user.userAge <= 0 || user.userAge >= 100);
    user.userId = gUniqueId++;
    fwrite(&user, sizeof(user), 1, filePtr);
    fclose(filePtr);
    return;
}

void displayUsers()
{
    FILE *filePtr = fopen(FILE_NAME, "rb");
    if(filePtr == NULL)
    {
        printf("\nError While Opening File!");
        return;
    }
    while(fread(&user, sizeof(user), 1, filePtr) == 1)
    {
        printf("%d %s %d\n", user.userId, user.userName, user.userAge);
    }
    fclose(filePtr);
}

void updateUser()
{
    FILE *filePtr = fopen(FILE_NAME, "rb+");
    if(filePtr == NULL)
    {
        printf("\nError While Opening File!");
        return;
    }
    int currentId;
    printf("\nEnter User ID to Update: ");
    scanf("%d", &currentId);
    int found = 0;
    getchar();
    while(fread(&user, sizeof(user), 1, filePtr) == 1)
    {
        if(user.userId == currentId)
        {
            int result;
            do
            {
                printf("\nEnter New Name: ");
                result = scanf("%[^\n]", user.userName);
                getchar();
                if(result == 0){
                    printf("Invalid name! Please enter a valid name.\n");
                }
            }while(result == 0);
            do
            {
                printf("\nEnter New Age: ");
                scanf("%d", &user.userAge);
                getchar();
                if(user.userAge <= 0 || user.userAge >= 100){
                    printf("Invalid age! Please enter a number between 1 and 99.\n");
                }
            }while(user.userAge <= 0 || user.userAge >= 100);
                fseek(filePtr, -sizeof(user), SEEK_CUR);
                fwrite(&user, sizeof(user), 1, filePtr);
                found = 1;
                break;
        }
    }
    if(found == 0)
    {
        printf("User Not Found!");
    }
    fclose(filePtr);
    return;
}


void deleteUser()
{
    FILE *filePtr = fopen(FILE_NAME, "rb");
    FILE *tmpPtr = fopen("Temp.DAT", "wb");
    if(filePtr == NULL)
    {
        printf("\nError While Opening File!");
        return;
    }
    if(tmpPtr == NULL)
    {
        printf("\nError While Opening File!");
        return;
    }
    int currentId;
    printf("\nEnter user Id to delete: ");
    scanf("%d", &currentId);

    int found = 1;
    while(fread(&user, sizeof(user), 1, filePtr) == 1)
    {
        if(currentId == user.userId)
        {
            found = 0;
            continue;
        }
        else
        {
            fwrite(&user, sizeof(user), 1, tmpPtr);
        }
    }
    fclose(filePtr);
    fclose(tmpPtr);
    if(found == 0)
    {
        printf("\nRecord Deleted Successfully!");
    }
    else
    {
        printf("\nRecord Not Found!");
    }
    remove("Users.DAT");
    rename("Temp.DAT", "Users.DAT");
}

int inf = 1;

int main()
{
    assignId();
    while(inf)
    {
        displayMenu();
        int ch;
        scanf("%d", &ch);
        getchar();
        switch(ch)
        {
            case 1:
                addUser();
                break;
            case 2:
                displayUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                inf = 0;
                break;
            default:
                printf("\nInvalid Option. Enter Again!");
                break;   
        }
    }
}
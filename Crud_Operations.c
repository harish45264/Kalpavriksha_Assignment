#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define FILE_NAME "Users.DAT"

void displayMenu(){
    printf("\n1. Add User Data\n");
    printf("2. Show All User Data\n");
    printf("3. Update User Data\n");
    printf("4. Delete User Data\n");
    printf("5. Exit\n");
    printf("Enter your Choice: ");
}

struct User{
    int id;
    char name[100];
    int age;
};

struct User e;

int unique_id = 0;
int size = sizeof(e);


void assignId(){
    FILE *fp = fopen(FILE_NAME, "rb");
    if(fp == NULL){
        printf("\nError While Opening File! OR No Such File Found");
        return;
    }
    while(fread(&e, size, 1, fp) == 1){
        unique_id = e.id;
    }
    unique_id++;
    fclose(fp);
}

void addUser(){
    FILE *fp = fopen(FILE_NAME,"ab");
    if(fp == NULL){
        printf("\nError While Opening File!");
        return;
    }
    printf("\nEnter Name: ");
    scanf("%[^\n]", e.name);
    printf("\nEnter Age: ");
    scanf("%d", &e.age);
    e.id = unique_id++;
    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);
    return;
}

void displayUsers(){
    FILE *fp = fopen(FILE_NAME, "rb");
    if(fp == NULL){
        printf("\nError While Opening File!");
        return;
    }
    while(fread(&e, size, 1, fp) == 1){
        printf("%d %s %d\n", e.id, e.name, e.age);
    }
    fclose(fp);
}

void updateUser(){
    FILE *fp = fopen(FILE_NAME, "rb+");
    if(fp == NULL){
        printf("\nError While Opening File!");
        return;
    }
    printf("\nEnter User ID to Update: ");
    int cur_id;
    scanf("%d", &cur_id);
    int found = 0;
    getchar();
    while(fread(&e, size, 1, fp) == 1){
        if(e.id == cur_id){
            int cur_id = e.id;
            printf("\nEnter New Name: ");
            scanf("%[^\n]", e.name);
            printf("\nEnter New Age: ");
            scanf("%d", &e.age);
            fseek(fp, -size, SEEK_CUR);
            fwrite(&e, size, 1, fp);
            found = 1;
            break;
        }
    }
    if(found == 0){
        printf("User Not Found!");
    }
    fclose(fp);
}


void deleteUser(){
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *tmp = fopen("Temp.DAT", "wb");
    if(fp == NULL){
        printf("\nError While Opening File!");
        return;
    }
    if(tmp == NULL){
        printf("\nError While Opening File!");
        return;
    }
    char name[100];
    printf("\nEnter User Name To Delete: ");
    scanf("%[^\n]", name);

    int f = 1;
    while(fread(&e, size, 1, fp) == 1){
        if(_stricmp(e.name, name) == 0){
            f = 0;
            printf("\nRecord Deleted Successfully!");
            continue;
        }
        else{
            fwrite(&e, size, 1, tmp);
        }
    }

    fclose(fp);
    fclose(tmp);
    if(f == 0){
        return;
    }
    else{
        printf("\nRecord Not Found!");
    }
    remove(FILE_NAME);
    rename("Temp.DAT", FILE_NAME);
}

int inf = 1;

int main(){
    assignId();
    while(inf){
        displayMenu();
        int ch;
        scanf("%d", &ch);
        getchar();
        switch(ch){
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
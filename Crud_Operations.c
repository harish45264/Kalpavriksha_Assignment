#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void displayMenu(){
    printf("\n1. Add Employee Data\n");
    printf("2. Show All Employee Data\n");
    printf("3. Update Employee Data\n");
    printf("4. Delete Employee Data\n");
    printf("5. Exit\n");
    printf("Enter your Choice: ");
}

struct Employee{
    int id;
    char name[100];
    int age;
};

struct Employee e;

int unique_id = 0;
int size = sizeof(e);


void assignId(){
    FILE *fp = fopen("Employee.DAT", "rb");
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

void addEmployee(){
    FILE *fp = fopen("Employee.DAT","ab");
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

void displayEmployees(){
    FILE *fp = fopen("Employee.DAT", "rb");
    if(fp == NULL){
        printf("\nError While Opening File!");
        return;
    }
    while(fread(&e, size, 1, fp) == 1){
        printf("%d %s %d\n", e.id, e.name, e.age);
    }
    fclose(fp);
}

void updateEmployee(){
    FILE *fp = fopen("Employee.DAT", "rb+");
    if(fp == NULL){
        printf("\nError While Opening File!");
        return;
    }
    printf("\nEnter Employee Name to Update: ");
    char name[100];
    scanf("%[^\n]", name);
    while(fread(&e, size, 1, fp) == 1){
        if(_stricmp(e.name, name) == 0){
            int cur_id = e.id;
            printf("\nEnter New Name: ");
            scanf("%[^\n]", e.name);
            printf("\nEnter New Age: ");
            scanf("%d", &e.age);

            fseek(fp, -size, SEEK_CUR);
            fwrite(&e, size, 1, fp);
            break;
        }
    }
    fclose(fp);
}


void deleteEmployee(){
    FILE *fp = fopen("Employee.DAT", "rb");
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
    printf("\nEnter Employee Name To Delete: ");
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
    remove("Employee.DAT");
    rename("Temp.DAT", "Employee.DAT");
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
                addEmployee();
                break;
            case 2:
                displayEmployees();
                break;
            case 3:
                updateEmployee();
                break;
            case 4:
                deleteEmployee();
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
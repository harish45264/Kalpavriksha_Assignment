#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100

struct student
{
    int rollNumber;
    char studentName[50];
    int studentMarks[3];
    int totalMark;
    float averageMark;
    char studentGrade;
};

struct student *studentArray;

int main(){
    int numberOfStudents;
    printf("\nEnter number of Students: ");
    scanf("%d", &numberOfStudents);
    getchar();
    studentArray = malloc(numberOfStudents * sizeof(struct student));
    for(int index = 0; index < numberOfStudents; index++){
        char inputLine[100];
        fgets(inputLine, sizeof(inputLine), stdin);
        sscanf(inputLine ,"%d %s %d %d %d", &studentArray[index].rollNumber, 
                                studentArray[index].studentName,
                                &studentArray[index].studentMarks[0],
                                &studentArray[index].studentMarks[1],
                                &studentArray[index].studentMarks[2]);
        studentArray[index].totalMark = studentArray[index].studentMarks[0] 
                                + studentArray[index].studentMarks[1] 
                                + studentArray[index].studentMarks[2];
    }
    for(int index = 0; index < numberOfStudents; index++){
        printf("%d %s %d\n", studentArray[index].rollNumber, studentArray[index].studentName, studentArray[index].totalMark);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

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
static int gNumberOfStudents;

char findGrade (float currentAverage)
{
    if (currentAverage >= 85) return 'A';

    else if (currentAverage >= 70) return 'B';

    else if (currentAverage >= 50) return 'C';

    else if (currentAverage >= 35) return 'D';

    return 'F';
}

bool isValidRoll (int rollNumber)
{
    return (rollNumber < 0 || rollNumber > 100) ? false : true;
}

int totalMarks (struct student currentStudent)
{
    return (currentStudent.studentMarks[0] 
                                + currentStudent.studentMarks[1] 
                                + currentStudent.studentMarks[2]);
}

float averageMarks (struct student currentStudent)
{
    return (currentStudent.studentMarks[0] 
                                + currentStudent.studentMarks[1] 
                                + currentStudent.studentMarks[2]) / 3.0;
}

int getCount (char currentGrade)
{
    if (currentGrade == 'A') return 5;

    if (currentGrade == 'B') return 4;

    if (currentGrade == 'C') return 3;

    if (currentGrade == 'D') return 2;

    return 0;
}

void displayStudents () 
{
    for (int index = 0; index < gNumberOfStudents; index++)
    {
        printf("Roll: %d\n",studentArray[index].rollNumber);
        printf("Name: %s\n",studentArray[index].studentName);
        printf("Total: %d\n",studentArray[index].totalMark);
        printf("Average: %.2f\n",studentArray[index].averageMark);
        printf("Grade: %c\n",studentArray[index].studentGrade);
        if(studentArray[index].averageMark >= 35){
            int counter = getCount(studentArray[index].studentGrade);
            printf("Performance: ");
            for (int asterisk = 0; asterisk < counter; asterisk++)
            {
                printf("*");
            }
        }
        printf("\n");
        printf("\n");
    }
}

void displayRollNumberHelper (int countOfStudents)
{
    if (countOfStudents == 1)
    {
        printf("%d ", countOfStudents);
        return;
    }
    displayRollNumberHelper(countOfStudents - 1);
    printf("%d ", countOfStudents);
}

void displayRollNumber (int countOfStudents) 
{
    printf("List of roll numbers: ");
    displayRollNumberHelper(countOfStudents);
}

int main() 
{
    printf("\nEnter number of Students: ");
    scanf("%d", &gNumberOfStudents);
    getchar();
    studentArray = malloc(gNumberOfStudents * sizeof(struct student));
    for (int index = 0; index < gNumberOfStudents; index++){
        char inputLine[100];
        int inputCount = 0;
        do
        {
            fgets(inputLine, sizeof(inputLine), stdin);
            inputLine[strcspn(inputLine, "\n")] = '\0';
            inputCount = sscanf(inputLine ,"%d %99s %d %d %d", &studentArray[index].rollNumber, 
                                    studentArray[index].studentName,
                                    &studentArray[index].studentMarks[0],
                                    &studentArray[index].studentMarks[1],
                                    &studentArray[index].studentMarks[2]);
            if (inputCount != 5) 
            {
                printf("Enter valid input format!\n");
            }
            if (!isValidRoll(studentArray[index].rollNumber))
            {
                printf("Enter valid roll number!\n");
                inputCount--;
            }
        } while (inputCount != 5);

        studentArray[index].totalMark = totalMarks(studentArray[index]);
        studentArray[index].averageMark = averageMarks(studentArray[index]);
        studentArray[index].studentGrade = findGrade(studentArray[index].averageMark);
    }
    displayStudents();
    displayRollNumber(gNumberOfStudents);
}
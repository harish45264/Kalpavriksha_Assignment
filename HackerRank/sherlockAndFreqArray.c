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


char* isValid(char* string);

char* isValid(char* string) {
    int *frequency = calloc(26, sizeof(int));
    int position = 0;
    while(string[position] != '\0')
    {
        frequency[string[position] - 'a']++;
        position++;
    }
    int minFrequency = INT_MAX, maxFrequency = -1;
    for(int position = 0; position < 26; position++){
        if(frequency[position] == 0) continue;
        if(frequency[position] > maxFrequency) maxFrequency = frequency[position];
        if(frequency[position] < minFrequency) minFrequency = frequency[position];
    }
    int minFreqCount = 0, maxFreqCount = 0;
    for(int position = 0; position < 26; position++){
        if(frequency[position] == 0) continue;
        else if(frequency[position] == minFrequency) minFreqCount++;
        else if(frequency[position] == maxFrequency) maxFreqCount++;
        else return "NO";
    }
    if (minFrequency == maxFrequency){
        return "YES";
    }
    if(minFrequency + 1 == maxFrequency && maxFreqCount == 1){
        return "YES";
    }
    if(minFrequency == 1 && minFreqCount == 1){
        return "YES";
    }
    return "NO";
}

int main()
{
    char* string = malloc(100 * sizeof(char));
    printf("Enter a Word: ");
    scanf("%s", string);
    char* result = isValid(string);
    printf("%s", result);
    return 0;
}

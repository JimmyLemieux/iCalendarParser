#include <stdio.h>
#include <string.h>
#include <ctype.h>
// #include "CalendarParser.h"
// #include "LinkedListAPI.h"
#include "CalendarParser.h"


// Make copy, strcat, copyTO, returns index of char and substring

// char * stringCpy(char * first, char * second) {
//     if(first == NULL || second == NULL) {
//         return NULL;
//     }
//     first = (char *)realloc(first, strlen(second) + 1);
//     for(int i = 0;i<strlen(second);i++) {
//         first[i] = second[i];
//     }
//     first[strlen(second)] = '\0';
//     return first;
// }

// char *stringCat(char *first,char *second) {
//     char *result;
//     int k = 0;
//     if(first == NULL || second == NULL) {
//         return NULL;
//     }
    
//     result = (char *)calloc(strlen(first) + strlen(second) + 1, sizeof(char));

//     result = stringCpy(result,first);
//      for(int i = strlen(result);i<strlen(first) + strlen(second);i++) {
//          result[i] = second[k]; 
//          k++;
//      }

//     return result;
// }


// int indexOf(char *line, char c) {
//     int index = 0;
//     if(line == NULL) {
//         return -1;
//     }
//     while(index < strlen(line) && line[index] != c) {
//         index++;
//     }
//     if(index == strlen(line)) {
//         return -1;
//     }
//     return index;
// } 

// int indexOfSubstring(char *line, char *substring) {
//     char *tempSub;
//     if(line == NULL || substring == NULL) {
//         return -1;
//     }
//     if(strlen(substring) > strlen(line)) {
//         return -1;
//     }
    
// }

void stringToLower(char *string) {
    int index = 0; 
    if(string == NULL) {
        return;
    }
    for(index = 0;string[index];index++) {
        string[index] = ((unsigned char)tolower(string[index]));
    }
}

int main() {
    char *a;
    a = calloc(1, 10 * sizeof(char));
    strcpy(a,"TEST");
    stringToLower(a);
    printf("%s\n", a);
    //printf("%s\n", a);
    //createCalendar("test.ics", NULL); 
    //printf("Error Code: %u\n", createCalendar("badFile.ical", NULL));
    return 0;
}

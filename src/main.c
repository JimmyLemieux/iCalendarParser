#include <stdio.h>
#include <string.h>
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




int main() {
    createCalendar("test.ics", NULL); 
    //printf("Error Code: %u\n", createCalendar("badFile.ical", NULL));
    return 0;
}

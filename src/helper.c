#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

/* Making helper functions that will make my life easier */

// Make copy, strcat, copyTO, returns index of char and substring



int checkToken(char *token) {
    for(int i = 0;i<strlen(token);i++) {
        if(token[i] == '\n' || token[i] == '\r') {
            return 0;
        }
    }
    return 1;
}


char * trimSpecialChars(char *line) {
    int i;
    if(line == NULL){
        return NULL;
    }
    //Remove the \r\n
    line[strlen(line) - 2] = '\0';    
    return line;
}


char *setStr(char *s) {
    char *new = calloc(1, sizeof(char) *strlen(s) + 9);
    strcpy(new,s);
    strcat(new,"\0");
    return new;
}


char ** init_str_array(int N, int K) {
    char **a;
    int i;

    a = malloc(sizeof(char *) * N);

    for(i = 0;i<N;i++) {
        a[i] = malloc(sizeof(char) * K);
    }
    return a;
}

void free_fields(char **ptr, int size) {
    int i;
    for(i = 0;i<size;i++) {
        free(ptr[i]);
    }
    free(ptr);
}

void stringToLower(char *string) {
    int index = 0; 
    if(string == NULL) {
        return;
    }
    for(index = 0;string[index];index++) {
        string[index] = ((unsigned char)tolower(string[index]));
    }
}

void stringToUpper(char *string) {
    int index = 0;
    if(string == NULL) {
        return;
    }
    for(index=0;string[index];index++) {
        string[index] = ((unsigned char)toupper(string[index]));
    }
}




char * stringCpy(char * first, char * second) {
    if(first == NULL || second == NULL) {
        return NULL;
    }
    first = (char *)realloc(first, strlen(second) + 1);
    for(int i = 0;i<strlen(second);i++) {
        first[i] = second[i];
    }
    first[strlen(second)] = '\0';
    return first;
}

char *stringCat(char *first,char *second) {
    char *result;
    int k = 0;
    if(first == NULL || second == NULL) {
        return NULL;
    }
    
    result = (char *)calloc(strlen(first) + strlen(second) + 1, sizeof(char));

    result = stringCpy(result,first);
     for(int i = strlen(result);i<strlen(first) + strlen(second);i++) {
         result[i] = second[k]; 
         k++;
     }

    return result;
}


int indexOf(char *line, char c) {
    int index = 0;
    if(line == NULL) {
        return -1;
    }
    while(index < strlen(line) && line[index] != c) {
        index++;
    }
    if(index == strlen(line)) {
        return -1;
    }
    return index;
}


int indexOfSubstring(char *line, char *substring) {
    char *tempSub;
    if(line == NULL || substring == NULL) {
        return -1;
    }
    if(strlen(substring) > strlen(line)) {
        return -1;
    }
    return 0;    
}




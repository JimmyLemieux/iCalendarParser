#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#define SEG printf("This line is giving a Segfault")
#define FREE free(x)
#define MAL malloc(size_t s)
#define PRINT printf(x)
#define CAL calloc(n,size_t s)
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


char ** initStrArray(int N, int K) {
    char **a;
    int i;

    a = malloc(sizeof(char *) * N);

    for(i = 0;i<N;i++) {
        a[i] = malloc(sizeof(char) * K);
    }
    return a;
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
    if(line == NULL || substring == NULL) {
        return -1;
    }
    if(strlen(substring) > strlen(line)) {
        return -1;
    }
    return 0;    
}


int isStringEmpty(char *string) {
    if(string == NULL || strlen(string) == 0) {
        printf("The String is empty\n");
        return 1;
    }
    return 0;
}


void splitByFirstOccurence(char *line, char *left, char *right, char del) {
    int i;
    int k;
    int find = indexOf(line,del);

    if(find == -1) {
        return;
    }
    for(i = 0;i<find;i++) {
        left[i] = line[i];
    }
    
    for(k = find+1,i=0;k<strlen(line);k++,i++) {
        right[i] = line[k];
    }

}

void deallocator(void *thingToFree) {
    if(thingToFree != NULL) {
       // printf("Freeing %s\n", (char *)thingToFree);
        free(thingToFree);
        thingToFree = NULL;
    }
}

void free_fields(char **ptr, int size) {
    int i;
    for(i = 0;i<size;i++) {
        free(ptr[i]);
    }
    free(ptr);
}

int isEmpty(char * string) {
    if(string == NULL || strlen(string) == 0) {
        return 1;
    }
    return 0;
}


int containsChar(char *line,char c) {
    int i;
    for(i = 0;i<strlen(line);i++) {
        if(line[i] == c) {
            return 1;
        }
    }
    return 0;
}

int checkBefore(char *line , char del1, char del2) {

    int index1 = indexOf(line,';');
    int index2 = indexOf(line,':');
    if(index1 < index2 && (index1 != -1) && (index2 != -1)) {
        return 1;
    }
    return 0;
}


void removeFirstChar(char *temp) {
    if(temp == NULL) {
        return;
    }
    temp += 1;
    printf("%s",temp); 
}

void splitContentLine(char *line, char *left, char *right) {
    if(left == NULL || right == NULL || line == NULL) {
        return;
    }
    if(containsChar(line, ';') && checkBefore(line,';',':')) {
    /* We are going to split by first occurence of the ; */
    splitByFirstOccurence(line, left,right,';');

    } else {
        /* we are going to split by first occurence of the : */
        splitByFirstOccurence(line, left,right,':');
    }

}


int containsSubstring(char *line, char *sub) {
    if(line == NULL || sub == NULL) {
        return 0;
    }

    if(strlen(sub) > strlen(line)) {
        return 0;
    }

    char *temp = calloc(1, sizeof(char) * strlen(sub));

    for(int i = 0;i<=strlen(line) - strlen(sub);i++) {
        for(int j = i,k=0;j < i + strlen(sub);j++,k++) {
            temp[k] = line[j];
        }

        if(strcasecmp(temp,sub) == 0) {
            deallocator(temp);
            return 1;
        }
    }
    deallocator(temp);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "CalendarParser.h"
// #include "LinkedListAPI.h"


Calendar cal;
List list;


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

int checkToken(char *token) {
    for(int i = 0;i<strlen(token);i++) {
        if(token[i] == '\n' || token[i] == '\r') {
            return 0;
        }
    }
    return 1;
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


void  splitLine(char *fileFeed,char* del, int *arraySize) {
    char *token;
    char *tempLine;
    char **fileLines;
    int j;
    if(fileFeed == NULL || del == NULL) {
        return;
    }


    token = (char *)malloc(sizeof(char) * 256);
    token = strtok(fileFeed, del);
    j = 0;
    while(token != NULL) {
        if(j % 255 == 0 && j != 0) {
            fileLines = (char **)realloc(fileLines, sizeof(char *)* 255);
        }
        strcat(token,"\0");
        tempLine = (char *)malloc(strlen(token)+1);
        strcpy(tempLine,token); //includes the null ter from token
        fileLines[j] = (char *)malloc(strlen(tempLine) + 1);
        strcpy(fileLines[j],tempLine);
        token = strtok(NULL,del); 
        j++;
    }
    *arraySize = j;
    free(tempLine);
    tempLine = NULL;
}

/*Make a trim that will remove white space,
If the line has a space at the begin and is a potential line fold then do a special case for the trim */

char * trimLine(char *line) {
    if(line == NULL){
        return NULL;
    }
    //Remove the \r\n
}



char *setStr(char *s) {
    char *new = calloc(1, sizeof(char) *strlen(s) + 9);
    strcpy(new,s);
    strcat(new,"\0");
    return new;
}




char ** readFileChar(char *fileName, int *arraySize,int *fileLines) {

    int c;
    int fileLineCount = 0;
    int lineSize = 0;
    int stringSize = 0;
    int index = 0;
    int tempStart = 0;
    int i;
    int k;
    char *stringBuffer = calloc(1,sizeof(stringBuffer));
    char *temp; 
    char **lines = calloc(1, sizeof(lines)*lineSize + 1);
    
    FILE *file = fopen(fileName,"r");


    /* This reads in the file line by line */
    while((c = fgetc(file)) != EOF) {
        stringBuffer[stringSize] = (char)c;
        if((char) c == '\n') fileLineCount++; 
        stringSize++;
        stringBuffer = realloc(stringBuffer,sizeof(stringBuffer) * (stringSize) + 1);
        stringBuffer[stringSize] = '\0';
    } //End of reading from the array

    while(index < strlen(stringBuffer) - 1) {

        if(stringBuffer[index] == '\r') {
            if(stringBuffer[index+1] == '\n') {
                k = 0;
                int startIndex = tempStart;
                int endIndex = index+2;
                temp = calloc((index+3), sizeof(temp));
                lines[lineSize] = calloc(100,sizeof(char));
                strncpy(temp, stringBuffer + startIndex, endIndex - startIndex);
                lines[lineSize] = setStr(temp);
                strcpy(lines[lineSize],temp);
                //printf("Temp value -> %s",temp);
                index+=2;
                tempStart = index;
                lineSize++;
                lines = realloc(lines, sizeof(lines) * (lineSize+1));
                free(temp);
                temp = NULL;
                continue;
            }

        }
        index++;
    }
    *fileLines = fileLineCount;
    *arraySize = lineSize;
    fclose(file);
    return lines;
}


ICalErrorCode validateFileLines(char **lines, int arraySize, int fileLines) {
    //Declare vars
    int i;
    int j;

    if(lines == NULL || (arraySize < fileLines)) {
        printf("Bad Line\n");
        printf("This is an invalid file\n");
        return INV_FILE;
    }
    for(i = 0;i<arraySize;i++) {
        if(lines[i][strlen(lines[i]) - 1] != '\n' || lines[i][strlen(lines[i]) - 2] != '\r') {
            return INV_FILE;
        }
    }
    return OK;
}




ICalErrorCode checkCalendarHead(char **lines, int arraySize) {
    //Looking for a BEGIN:VCALENDER and then an END:VCALENDAR
    //The very first line in the file needs to be a BEGIN:CALENDAR




}

ICalErrorCode createCalendar(char* fileName, Calendar** obj) {
    /*First step, when opening the file make sure it is of valid file extension
    Also make sure that the actual file opens and that you can read contents from the file
    */
    char **lines;
    char *tempFile;
    char *fileExtension;
    FILE *file;
    int i;
    int j;
    int index;
    int errnum;
    int arraySize;
    int fileLines;
    char *stringBuffer = calloc(1,sizeof(stringBuffer));

    obj = malloc(sizeof(Calendar*));
    *obj = malloc(sizeof(Calendar));
    

    if(fileName == NULL) {
        printf("The file name is null\n");
        return INV_FILE;
    }

    tempFile = (char *)malloc(strlen(fileName) + 1);
    strcpy(tempFile,fileName);

    index = 0;

    while(index < strlen(tempFile) && tempFile[index] != '.') {
        index++;
    }

    if(index == strlen(tempFile)) {
        printf("There is no file extension\n");
        return INV_FILE;
    }

    if(tempFile[index] == '.') {
        j = 0;
        index++;
        fileExtension = (char *)malloc((strlen(tempFile) - index) + 1);
        while(index < strlen(tempFile)) {
            fileExtension[j] = tempFile[index];
            index++;
            j++;
        }
        fileExtension[j] = '\0';
        if(strcmp(fileExtension,"ics") != 0) {
            printf("The file is not a ical file\n"); 
            return INV_FILE;
        }
    }

    //Open the file and read the contents line by line

    file = fopen(tempFile,"r");
    if(file == NULL) {  //The file did not open properly
        errnum = errno;
        printf("There was an error on the file load\n");
        fprintf(stderr,"Error opening file: %s\n",strerror(errnum));
        return INV_FILE;
    }
    i = 0;

    fclose(file);



    char **test = readFileChar(tempFile, &arraySize,&fileLines);//This needs to be freed and checked for memleaks

    printf("arraySize: %d and fileLines: %d\n", arraySize,fileLines);

    validateFileLines(test,arraySize,fileLines); // Validation of the lines in the file and the tokenizer






    free(tempFile);
    free(fileExtension);
    return OK;
}


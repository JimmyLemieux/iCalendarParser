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

/*Make a trim that will remove white space,
If the line has a space at the begin and is a potential line fold then do a special case for the trim */

char * trimLine(char *line) {
    if(line == NULL){
        return NULL;
    }
    //Remove the \r\n
    return NULL;
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
                //lines[lineSize] = calloc(1,strlen(temp) * 9);
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
    free(stringBuffer);
    return lines;
}


ICalErrorCode validateFileLines(char **lines, int arraySize, int fileLines) {
    //Declare vars
    int i;
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
    /* Push worked from the remote */

    return OK;
}

ICalErrorCode createCalendar(char* fileName, Calendar** obj) {
    /*First step, when opening the file make sure it is of valid file extension
    Also make sure that the actual file opens and that you can read contents from the file
    */
    char *tempFile = NULL;
    char *fileExtension = NULL;
    FILE *file;
    int i;
    int j;
    int index;
    int errnum;
    int arraySize;
    int fileLines;

    // obj = malloc(sizeof(Calendar*));
    // *obj = malloc(sizeof(Calendar));
    

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
        free(tempFile);
        free(fileExtension);
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
        printf("The file extension is %s\n",fileExtension);

        if(strcmp(fileExtension,"ics") != 0) {
            free(fileExtension);
            free(tempFile);
            printf("The file is not a ical file\n"); 
            return INV_FILE;
        }
    }


    //Open the file and read the contents line by line

    printf("The file name is %s\n",fileName); 
    file = fopen(fileName,"r");
    if(file == NULL) {  //The file did not open properly
        errnum = errno;
        printf("There was an error on the file load\n");
        fprintf(stderr,"Error opening file: %s\n",strerror(errnum));
        return INV_FILE;
    }

    //End of pen test for file
    fclose(file);

    i = 0;


    char **test = readFileChar(tempFile, &arraySize,&fileLines);//This needs to be freed and checked for memleaks

    printf("arraySize: %d and fileLines: %d\n", arraySize,fileLines);

    validateFileLines(test,arraySize,fileLines); // Validation of the lines in the file and the tokenizer


    //Free
    for(i = 0;i<arraySize;i++) {
        printf("%s",test[i]);
        free(test[i]);
    }


    free(test);
    free(tempFile);
    free(fileExtension);
    return OK;
}


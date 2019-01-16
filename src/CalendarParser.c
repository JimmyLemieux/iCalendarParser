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


/* This function will trim the \r\n from the end of each line and */

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

void free_fields(char **ptr, int size) {
    int i;
    for(i = 0;i<size;i++) {
        free(ptr[i]);
    }
    free(ptr);
}



//This works with big input
char** readFileChar(char *fileName, int *arraySize,int *fileLines) { //Cool tokenizer and memleak fix
    int c;
    int fileLineCount = 0;
    int lineSize = 0;
    int stringSize = 0;
    int index = 0;
    int tempStart = 0;
    int k;
    char *stringBuffer = calloc(1,sizeof(stringBuffer));
    char *temp; 
    char **lines = calloc(1, sizeof(char *));
    
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

                temp = calloc(1, sizeof(char*)*(endIndex - startIndex));                
                
                //strcpy(temp,"Hello");
                strncpy(temp, stringBuffer + startIndex, endIndex - startIndex);
                
                lines[lineSize] = calloc(1,strlen(temp) + 1);
                strcpy(lines[lineSize],temp);
                lineSize++;
                lines = realloc(lines, sizeof(lines) * (lineSize+1));
                
                
                
                //For the loop
                index+=2;
                tempStart = index;

                //Free the string
                free(temp);
                temp = NULL;
                continue;
            }

        }
        index++;
    }

    free(stringBuffer);
    free(temp);

    *fileLines = fileLineCount;
    *arraySize = lineSize;
    fclose(file);


    // for(k=0;k<lineSize;k++) {
    //     printf("%s",lines[k]);
    // }


    return lines;
}


ICalErrorCode validateFileLines(char **lines, int arraySize, int fileLines) {
    //Declare vars
    int i;
    if(lines == NULL || (arraySize < fileLines) || fileLines == 0 || arraySize == 0) {
        printf("This is an invalid file!\n");
        return INV_FILE;
    }
    for(i = 0;i<arraySize;i++) {
        if(lines[i][strlen(lines[i]) - 1] != '\n' && lines[i][strlen(lines[i]) - 2] != '\r') {
            printf("This is an invalid file due to no CRLF\n");
            return INV_FILE;
        }
    }
    return OK;
}




//This function will be for checking if the first line is BEGIN:VCALENDAR and the last line is END:VCALENDAR
//Also that the CALENDAR has all of its required properties in the top level directory
//This will require keeping track of BEGIN and END TAGS
//It is possible to do both of these at the same time :)
ICalErrorCode checkCalendarHead(char **lines, int arraySize) {
    //DEC VARS
    int i;
    int j;
    int k;
    int index;
    int foundVersion = 0;
    int foundPRODID = 0;
    int open = 0;
    char *left;
    char *right;

    if(lines == NULL || arraySize == 0) {
        printf("Something is still wrong with the file\n");
        return INV_FILE;
    }
    //Check to see if the CALENDAR has a BEGIN and then END, IF THERE is at least ONE COMPONENT
    //CHECK IF THE CALENDAR has a version and a PROID

    /* Checking if the first and last lines are valid */

    if(strcmp(lines[0],"BEGIN:VCALENDAR") != 0 || strcmp(lines[arraySize - 1], "END:VCALENDAR") != 0) {
        printf("The calendar does not start and end properly\n");
        return INV_CAL;
    }

    // Check for the begin and end tags are consistent
    //You just need to check for BEGIN or END VCALENDER within the first and last lines
    for(i = 1;i<arraySize - 1;i++) { //Looping through each line
        index = 0;
        while(index < strlen(lines[i]) && lines[i][index] != ':') {
            index++;
        }
        if(index == strlen(lines[i])) { // The : was not found
            continue;
        } 

        //The index is going to be where the left half begins
        left = calloc(1,index+1 * sizeof(left));
        right = calloc(1,(strlen(lines[i]) - index) * sizeof(char));
        for(k = 0;k<index;k++) {
            left[k] = lines[i][k];
        }
        //Since we have now found the right line, we can now find the right
        for(k = index+1,j=0;k<strlen(lines[i]);k++,j++) {
            right[j] = lines[i][k];
        }

        if((strcmp(left,"BEGIN") == 0 || strcmp(left,"END") == 0) && (strcmp(right,"VCALENDAR") == 0)) {
            printf("There is a duplicate property in the file\n");
            return INV_CAL;
        }
        free(left);
        free(right);
    }

    //If we made it through the above iterations then check if the calendar has a version and UID
    //If the calendar has both of these things in the top directory then you want to parse them

    for(i = 0;i<arraySize;i++) {
        index = 0;
        while(index < strlen(lines[i]) && lines[i][index] != ':') {
            index++;
        }
        if(index == strlen(lines[i])) {
            continue;
        }

        left = calloc(1,index+1 * sizeof(char));
        right = calloc(1,(strlen(lines[i]) - index) * sizeof(char));


        for(k = 0;k<index;k++) {
            left[k] = lines[i][k];
        }

        for(k = index+1,j=0;k<strlen(lines[i]);k++,j++) {
            right[j] = lines[i][k];
        }    

        if(open < 0) {
            printf("THERE WAS A BEGIN END MISMATCH");
            free(right);
            free(left);
            return INV_CAL;
        }

        if(strcmp(left,"BEGIN") == 0) {
            open++;
            continue; // to the next line
        }

        if(strcmp(left,"END") == 0) {
            open--;
            continue;
        }
        
        
        if(strcmp(left,"VERSION") == 0 && open == 1) {
            if(!foundVersion) {
                printf("Found version\n");
                foundVersion = 1;
            } else {
                free(left);
                free(right);
                return DUP_VER;
            }
        }

        if(strcmp(left,"PRODID") == 0 && open == 1) {
            if(!foundPRODID) {
                printf("Found the PRODID\n");
                foundPRODID = 1;
            } else {
                free(left);
                free(right);
                return DUP_PRODID;
            }
        }
        free(left);
        free(right);
    }


    if(!foundPRODID || !foundVersion) {
        return INV_CAL;
    } 


    return OK;
}


//This function is responsible for checking if there is at least one component in the CALENDAR
//If you can find a BEGIN:EVENT in the top level of the CALENDAR component this will return OK
//AN EVENT MUST have a BEGIN:VENEVT and then a END:VEVENT tag
//INSIDE an event it MUST have a DTSTAMP property and UID property and DTSTART property
ICalErrorCode checkForEvent(char **lines, int arraySize) {
    if(lines == NULL || arraySize == 0) {
        printf("This is an invalid file\n");
        return INV_FILE;
    }
    return OK;
}






ICalErrorCode createCalendar(char* fileName, Calendar** obj) { //Big mem leak fix on the tokenizer
    /*First step, when opening the file make sure it is of valid file extension
    Also make sure that the actual file opens and that you can read contents from the file
    */
    ICalErrorCode error;
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

        if(strcmp(fileExtension,"ics") != 0) {
            free(fileExtension);
            free(tempFile);
            printf("The file is not a ical file\n"); 
            return INV_FILE;
        }
    }


    //Open the file and read the contents line by line

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



    error = validateFileLines(test,arraySize,fileLines); // Validation of the lines in the file and the tokenizer


    if(error != 0) { //Error With the file
        printf("Invalid file\n");
        free_fields(test,arraySize);
        free(tempFile);
        free(fileExtension);
        return INV_FILE;
    }

    //If there is a pass, continue to look at the calendar contents
    // The calendar contents are supposed to be specified inside the text

    
    
    printf("\\THIS FILE WAS FLAGGED AS VALID\\\n");

    //Remove all of the special chars on each  line
    for(i = 0;i<arraySize;i++) {
        test[i] = trimSpecialChars(test[i]);
    }

    error = checkCalendarHead(test,arraySize);

    if(error != 0) {
        printf("This is an invalid file\n");
        free_fields(test,arraySize);
        free(tempFile);
        free(fileExtension);
        return INV_FILE;
    }

    printf("\\THIS IS A GOOD CALENDAR FILE!!\\\n");


    free_fields(test,arraySize);
    free(tempFile);
    free(fileExtension);
    return OK;
}


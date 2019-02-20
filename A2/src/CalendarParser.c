/* Lemieux James 1014181 jlemie03@uoguelph.ca */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <strings.h>
#include "LinkedListAPI.h"
#include "CalendarParser.h"
#include "helper.c"
#define D printf("debug\n")
#define R printf("Check for Return\n")
// #include "LinkedListAPI.h"

//Add a MISMATCH CHECKER that will check BEGIN and END tags

/* Starting the functions for the Linked List */

/* make the compare functions for the linked list */

/* The print functions are now compatible for the write calendar function */
char* printEvent(void *toBePrinted) {
	char *tempStr;
    char *temp;
	Event *tempEvent;
	if(toBePrinted == NULL) {
		return NULL;
	}

	tempEvent = (Event*)toBePrinted;

    /* Just print out all of the event things, not any of the alarm things */

    // if(tempEvent == NULL) {
    //     return NULL;
    // }
    tempStr = calloc(1, sizeof(char) * 100000);

    if(!isEmpty(tempEvent->UID)) {
        temp = calloc(1, sizeof(char) * strlen(tempEvent->UID) + 500);
        sprintf(temp, "UID:%s\r\n",tempEvent->UID);
        //tempStr = realloc(tempStr, sizeof(char) * strlen(temp));
        strcat(tempStr,temp);
        deallocator(temp);
    }

    /* Make a new temp string for the DTSTAMP and DTSTART, then from there make the properties */


    char *tempDTStamp;

    tempDTStamp = calloc(1, sizeof(char) * 100);
    strcpy(tempDTStamp, "DTSTAMP:");

    if(!isEmpty(tempEvent->creationDateTime.date)) {
        //temp = calloc(1,sizeof(char) * strlen(tempEvent->creationDateTime.date) + 500);
        tempDTStamp = realloc(tempDTStamp, sizeof(char) * (strlen(tempEvent->creationDateTime.date)) + 50);
        strcat(tempDTStamp,tempEvent->creationDateTime.date);
        //sprintf(temp,"DTSTAMPDATE:%s\r\n",tempEvent->creationDateTime.date);
        //tempStr = realloc(tempStr,sizeof(char) * strlen(temp));
        //strcat(tempStr,tem);
       // deallocator(temp);
    }


    if(!isEmpty(tempEvent->creationDateTime.time)) {
        //temp = calloc(1, sizeof(char) * strlen(tempEvent->creationDateTime.time) + 500);
        tempDTStamp = realloc(tempDTStamp, sizeof(char) * (strlen(tempEvent->creationDateTime.time)) + 51);
        strcat(tempDTStamp, "T");
        strcat(tempDTStamp,tempEvent->creationDateTime.time);
       // sprintf(temp,"DTSTAMPTIME:%s\r\n", tempEvent->creationDateTime.time);
        //tempStr = realloc(tempStr,sizeof(char) * strlen(temp));
        //strcat(tempStr,temp);
        //deallocator(temp);
    }

    if(tempEvent->creationDateTime.UTC) {
        strcat(tempDTStamp, "Z\r\n");
    } else {
        strcat(tempDTStamp, "\r\n");
    }


   // sprintf(temp,"DTSTAMPUTC:%d\r\n",tempEvent->creationDateTime.UTC);
    //tempStr = realloc(tempStr, sizeof(char) * strlen(temp) + 1);
   // strcat(tempStr, temp);
    strcat(tempStr, tempDTStamp);
    printf("The DTSTAMP is %s\n",tempDTStamp);
    deallocator(tempDTStamp); // Dont need this anymore

    

    char *tempDTStart = calloc(1, sizeof(char) * 100);

    strcpy(tempDTStart, "DTSTART:");

    if(!isEmpty(tempEvent->startDateTime.date)) {
        //temp = calloc(1,sizeof(char) * strlen(tempEvent->startDateTime.date) + 500);
        tempDTStart = realloc(tempDTStart, sizeof(char) * strlen(tempEvent->startDateTime.date) + 50);
        strcat(tempDTStart, tempEvent->startDateTime.date);
        //sprintf(temp,"DTSTARTDATE:%s\r\n",tempEvent->startDateTime.date);
        //tempStr = realloc(tempStr,sizeof(char) * strlen(temp));
        //strcat(tempStr,temp);
        //deallocator(temp);
    }


    if(!isEmpty(tempEvent->startDateTime.time)) {
        //temp = calloc(1, sizeof(char) * strlen(tempEvent->startDateTime.time) + 500);
        tempDTStart = realloc(tempDTStart, sizeof(char) * strlen(tempEvent->startDateTime.time) + 51);
        strcat(tempDTStart, "T");
        strcat(tempDTStart, tempEvent->startDateTime.time);
        //sprintf(temp,"DTSTARTTIME:%s\r\n", tempEvent->startDateTime.time);
        //tempStr = realloc(tempStr,sizeof(char) * strlen(temp));
        //strcat(tempStr,temp);
        //deallocator(temp);
    }

    if(tempEvent->startDateTime.UTC) {
        strcat(tempDTStart, "Z\r\n");
    } else {
        strcat(tempDTStart, "\r\n"); 
    }

//    temp = calloc(1, sizeof(char) * (500));
  //  sprintf(temp,"DTSTARTUTC:%d\r\n",tempEvent->startDateTime.UTC);
    strcat(tempStr, tempDTStart);
    deallocator(tempDTStart);
	return tempStr;
}
/* You will have to traverse all of the properties and alarms of this event as well */
/* You will need to free the two list * inside the event object */
int compareEvents(const void *first, const void *second) {
	return 0;
}

void deleteEvent(void *toBeDeleted) {
	Event *tempEvent;
	if(toBeDeleted == NULL) {
		return;
	}
	tempEvent = (Event*)toBeDeleted;
	/* We basically need to free everything that is contained inside the event object */
	/* for now just free the main event pointer */
    if(tempEvent->properties != NULL)
        freeList(tempEvent->properties);

    if(tempEvent->alarms != NULL)
        freeList(tempEvent->alarms);


    deallocator(tempEvent);
}

char *printAlarm(void *toBePrinted) {
    char *tempStr;
    Alarm *tempAlarm;
    int len;
    if(toBePrinted == NULL) {
        return NULL;
    }

    tempAlarm = (Alarm *)toBePrinted;
    len = strlen(tempAlarm->action) + strlen(tempAlarm->trigger) + 500;
    tempStr = calloc(1, sizeof(char) * len);
    sprintf(tempStr, "ACTION:%s\r\nTRIGGER:%s\r\n", tempAlarm->action, tempAlarm->trigger);
    return tempStr;
}

int compareAlarms(const void *first, const void *second) {
    return 0;
}

void deleteAlarm(void *toBeDeleted) {
    Alarm *tempAlarm;

    if(toBeDeleted == NULL) {
        return;
    }
    tempAlarm = (Alarm*)toBeDeleted;
    /* I will also have to go through the properties for this and free */
    freeList(tempAlarm->properties);
    free(tempAlarm->trigger);
    free(tempAlarm);
}

/*Ending the functions for the linked list */

char *printProperty(void *toBePrinted) {
    char * str;
    Property *tempProp;
    if(toBePrinted == NULL) {
        return NULL;
    }

    tempProp = (Property *)toBePrinted;


    str = calloc(1, sizeof(char) * 500);
    if(!isEmpty(tempProp->propName)) {
        str = realloc(str, sizeof(char) * strlen(tempProp->propName) + 500);
        sprintf(str, "%s:", tempProp->propName);
    }

    if(tempProp->propDescr[0] != 0 ||(!isEmpty(tempProp->propDescr))) {
        str = realloc(str, sizeof(char) * strlen(tempProp->propDescr) + 500);
        strcat(str,tempProp->propDescr);
    }
    return str;
}

int compareProperties(const void *first, const void *second) {
    return 0;
}

void deleteProperty(void *toBeDeleted) {
    Property *tempProp;

    if(toBeDeleted == NULL) {
        return;
    }

    tempProp = (Property *)toBeDeleted;
    free((Property*)tempProp);
}


char *printDate(void *toBePrinted) {

    char *str;
    int len = 0;

    DateTime *dateTime;

    if(toBePrinted == NULL) {
        return NULL;
    }

    dateTime = (DateTime*)toBePrinted;
    if(!isEmpty(dateTime->time)) {
        len += strlen(dateTime->time);
    } else if(!isEmpty(dateTime->date)) {
        len += strlen(dateTime->date);
    }
    str = calloc(1, sizeof(char) * len + 100);
    sprintf(str,"The time of the date is %s\nThe date is %s\nThe UTC is %d\n", dateTime->time,dateTime->time,dateTime->UTC);
    return str;
}


int compareDates(const void *first, const void *second) {
    return 0;
}

void deleteDate(void *toBeDeleted) {
    if(toBeDeleted == NULL) {
        return;
    }
}


/* Starting helper functions for basic file checking */

/* End the functions for basic file checking */

ICalErrorCode validateFile(char *fileName) {
    FILE *file;
    int j;
    int index;
    char *tempFile = NULL;
    char *fileExtension = NULL;

    if(fileName == NULL || strlen(fileName) == 0 || strcmp(fileName,".ics") == 0 || strcmp(fileName,"ics") == 0) {
        return INV_FILE;
    }

    tempFile = (char *)malloc(strlen(fileName) + 1);
    strcpy(tempFile,fileName);

    index = 0;

    while(index < strlen(tempFile) && tempFile[index] != '.') {
        index++;
    }

    if(index == strlen(tempFile)) {
        //printf("There is no file extension\n");
        deallocator(tempFile);
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
            //printf("The file is not a ical file\n");
            deallocator(fileExtension);
            deallocator(tempFile);
            return INV_FILE;
        }
    }

    //Open the file and read the contents line by line

    file = fopen(fileName,"r");
    if(file == NULL) {  //The file did not open properly
        deallocator(tempFile);
        deallocator(fileExtension);
        return INV_FILE;
    }

    fseek(file, 0, SEEK_END);
    if(ftell(file) == 0) {
        deallocator(tempFile);
        deallocator(fileExtension);
        fclose(file);
        return INV_FILE;
    }

    //End of pen test for file
    fclose(file);
    deallocator(tempFile);
    deallocator(fileExtension);
    return OK;
}


char ** lineUnfold(char **lines, int arraySize,int *contentSize){
    char **newLines;
    char *origLine;
    char *folded;
    char *tempFold;
    int lineCount = 0;

    if(lines == NULL || arraySize == 0) {
        return NULL;
    }


    newLines = malloc(sizeof(char *));
    for(int i = 0;i < arraySize - 1;) {
        if(isEmpty(lines[i])) {
            i++;
            continue;
        }


        origLine = calloc(1, sizeof(char) * (strlen(lines[i])) + 30);

        strcpy(origLine, lines[i]);

        //This will basically not remove the comments
        if(lines[i][0] == ';') {
            newLines = realloc(newLines, sizeof(char *) * (lineCount + 1));
            newLines[lineCount] = calloc(1, sizeof(char) * (strlen(origLine)) + 10);
            strcpy(newLines[lineCount], origLine);
            lineCount++;
            free(origLine);
            i++;
            continue;
        }
        int j = i + 1;

        if(lines[j][0] == ' ' || lines[j][0] == '\t' || lines[j][0] == ';') {
            while((j<arraySize && (isspace(lines[j][0]) || lines[j][0] == ';'))) {
                if(isEmpty(lines[j]) || lines[j][0] == ';') {
                    j++;
                    continue;
                }

                folded = calloc(1, sizeof(char) * (strlen(lines[j])) + 50);
                strcpy(folded,lines[j]);
                tempFold = removeFirstChar(folded);
                origLine = realloc(origLine,sizeof(char) * ((strlen(tempFold)) + strlen(origLine)) + 10);
                strcat(origLine, tempFold);
                free(folded);
                folded = NULL;
                free(tempFold);
                tempFold = NULL;

                j++;
            }
            i = j;
            newLines = realloc(newLines,sizeof(char *) * (lineCount+1));
            newLines[lineCount] = calloc(1, sizeof (char) * strlen(origLine) + 10);
            strcpy(newLines[lineCount], origLine);
            free(origLine);
            origLine = NULL;
            lineCount++;
            continue;
        }


        newLines = realloc(newLines, sizeof(char *) * (lineCount+1));
        newLines[lineCount] = calloc(1,sizeof(char) * (strlen(origLine))+ 10);
        strcpy(newLines[lineCount],origLine);
        lineCount++;
        i++;
        free(origLine);
        origLine = NULL;
    }

    if(strcasecmp(lines[arraySize - 1], newLines[lineCount - 1]) != 0) {
        newLines = realloc(newLines, sizeof(char *) * (lineCount + 1));
        newLines[lineCount] = calloc(1, sizeof(char) * (strlen(lines[arraySize - 1])) + 10);
        strcpy(newLines[lineCount],lines[arraySize - 1]);
        lineCount++;
    }


    // printf("first line of old lines - > %s\n", lines[0]);
    // printf("first line of new lines - > %s\n", newLines[0]);

    // printf("last line of old lines - > %s\n", lines[arraySize - 1]);
    // printf("last line of new lines - > %s\n", newLines[lineCount - 1]);

    *contentSize = lineCount;



    free_fields(lines,arraySize);
    return newLines;

}


/* Starting the helper functions for parsing the calendar  */


/* This function will remove the line folds and an empty lines */

/* This is for reading the file into a 2D array */
char** readFileChar(char *fileName, int *arraySize,int *fileLines) { //Cool tokenizer and memleak fix
    int c;
    int fileLineCount = 0;
    int lineSize = 0;
    int stringSize = 0;
    int index = 0;
    int tempStart = 0;
    char *stringBuffer = calloc(1,sizeof(stringBuffer));
    char *temp;
    char **lines = calloc(1, sizeof(char *));

    FILE *file = fopen(fileName,"r");

    if(file == NULL || fileName == NULL) {
        return NULL;
    }


    /* This reads in the file line by line */


    /* Check to see if there is anything to read in the file */

    while((c = fgetc(file)) != EOF) {
        stringBuffer[stringSize] = (char)c;
        stringSize++;
        stringBuffer = realloc(stringBuffer,sizeof(stringBuffer) * (stringSize) + 1);
        stringBuffer[stringSize] = '\0';
    } //End of reading from the array


    //Find the number of lines in the file
    int i = 0;
    for(i = 0;i<strlen(stringBuffer);i++) {
        if(stringBuffer[i] == '\n') {
            fileLineCount++;
        }
    }



    while(index < strlen(stringBuffer) - 1) {
        if(stringBuffer[index] == '\r') {
            if(stringBuffer[index+1] == '\n') {
                int startIndex = tempStart;
                int endIndex = index+2;
                temp = calloc(1, sizeof(char*)*(endIndex - startIndex) + 1);

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
                deallocator((char *)temp);
                temp = NULL;
                continue;
            }

        }
        index++;
    }


    if(lineSize == 0) {
        fclose(file);
        free(stringBuffer);
        free_fields(lines,lineSize);
        return NULL;
    }


    *fileLines = fileLineCount;
    *arraySize = lineSize;
    fclose(file);
    deallocator(temp);
    deallocator((char *)stringBuffer);
    return lines;
}


/* Used for simple validation of the file lines before I begin parsing and the basic file
contents before parsing */
ICalErrorCode validateFileLines(char **lines, int arraySize, int fileLines) {
    //Declare vars
    int i;
    if(lines == NULL || (arraySize < fileLines) || fileLines == 0 || arraySize == 0) {
        return INV_FILE;
    }
    for(i = 0;i<arraySize;i++) {
        if(lines[i][strlen(lines[i]) - 1] != '\n' && lines[i][strlen(lines[i]) - 2] != '\r') {
            return INV_FILE;
        }
    }
    return OK;
}


ICalErrorCode checkCalendarBeginEnd(char **lines, int arraySize) {
    int goodBeginEnd = 0;
    int count = 0;

    if(lines == NULL) {
        return OTHER_ERROR;
    }

    if(strcasecmp(lines[0],"BEGIN:VCALENDAR") == 0 && strcasecmp(lines[arraySize - 1],"END:VCALENDAR") == 0) {
        goodBeginEnd = 1;
    }

    for(int i = 0;i<arraySize;i++) {
        if(strcasecmp(lines[i],"BEGIN:VCALENDAR") == 0 || strcasecmp(lines[i], "END:VCALENDAR") == 0) {
            count++;
        }
    }

    if(count != 2 || !goodBeginEnd) {
        return INV_CAL;
    }

    return OK;
}


ICalErrorCode checkCalendarLayer(char **lines, int arraySize) {
    int foundVersion = 0;
    int foundPRODID = 0;
    int calComp = 0;
    int open = 0;
    char *left;
    char *right;


    if(lines == NULL || arraySize == 0) {
        return INV_FILE;
    }

    for(int i = 0;i<arraySize;i++) {


        if((!containsChar(lines[i],':') && open <= 1 && lines[i][0] != ';' && !isEmpty(lines[i])) || (isEmpty(lines[i]) && open <=1)) {
            return INV_CAL;
        }

        if(lines[i][0] == ';' && open <= 1) { // This is a comment line that is on cal scope or out of the calendar
            continue;
        }

        left = calloc(1, sizeof(char) * (strlen(lines[i])) + 10);
        right = calloc(1, sizeof(char) * (strlen(lines[i])) + 10);

        splitContentLine(lines[i], left, right);


        if(strcasecmp(left,"BEGIN") == 0) {
            if(strcasecmp(right,"VEVENT") == 0) {
                calComp++;
            }
            if(strcasecmp(right,"VEVENT") == 0 || strcasecmp(right,"VALARM") == 0 || strcasecmp(right,"VCALENDAR") == 0){
                open++;
            }
            deallocator((char *)left);
            deallocator((char *)right);
            continue; // to the next line
        }

        if(strcasecmp(left,"END") == 0 && (strcasecmp(right,"VALARM") == 0 || strcasecmp(right,"VEVENT") == 0 || strcasecmp(right,"VCALENDAR") == 0)) {
            open--;
            deallocator((char *)left);
            deallocator((char *)right);
            continue;
        }





        //You have to see if the PRODID and the Version tags are in the calendar
        if(strcasecmp(left,"PRODID") == 0 && open == 1) {
            if(isEmpty(right)) {
                deallocator(left);
                deallocator(right);
                return INV_PRODID;
            }
            if(foundPRODID == 1) {
                deallocator(left);
                deallocator(right);
                return DUP_PRODID;
            }
            foundPRODID = 1;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "VERSION") == 0 && open == 1) {
            if(isEmpty(right)) {
                deallocator(left);
                deallocator(right);
                return INV_VER;
            }
            if(foundVersion == 1) {
                deallocator(left);
                deallocator(right);
                return DUP_VER;
            }
            foundVersion = 1;
            deallocator(left);
            deallocator(right);
            continue;
        }

        deallocator(left);
        deallocator(right);
    }

    if(!foundVersion || !foundPRODID || !calComp) {
        return INV_CAL;
    }
    return OK;
}


ICalErrorCode checkBeginsAndEnds(char **lines, int arraySize) {
    int i;
    int j;
    char *right;
    char *left;
    const char validComponents[3][50] = { "VCALENDAR", "VEVENT", "VALARM" };


    if(lines == NULL || arraySize == 0) {
        return INV_CAL;
    }

    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i], ':') || lines[i][0] == ';') {
            continue;
        }

        left = calloc(1, sizeof(char) * (strlen(lines[i])) + 100);
        right = calloc(1, sizeof(char) * (strlen(lines[i])) + 100);


        splitContentLine(lines[i], left,right);

        if(strcasecmp(left, "BEGIN") == 0 || strcasecmp(left,"END") == 0) {
            int found = 0;
            for(j = 0;j<3;j++) {
                if(strcasecmp(validComponents[j], right) == 0) {
                    found = 1;
                    break;
                }
            }
            if(found == 1) {
                deallocator(left);
                deallocator(right);
                continue;
            } else {
                deallocator(left);
                deallocator(right);
                return INV_CAL;
            }

        }
        deallocator(left);
        deallocator(right);

    }
    return OK;
}


ICalErrorCode checkEventBeginEnd(char **lines, int arraySize) {
    int i;
    int j;
    char *right;
    char *left;
    int beginFound = 0;
    if(lines == NULL || arraySize == 0) {
        return OTHER_ERROR;
    }

    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i],':') ||  lines[i][0] == ';') {
            continue;
        }
        left = calloc(1,sizeof(char) * strlen(lines[i]) + 500);
        right = calloc(1,sizeof(char) * strlen(lines[i]) + 500);
        /* The string contains the char */


        splitContentLine(lines[i],left,right);

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VEVENT") == 0 && !beginFound) {
            deallocator(left);
            deallocator(right);
            return INV_CAL;
        }

        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right,"VEVENT") == 0) {
            beginFound = 1;
            int endFound = 0;
            j = i+1;
            while(j < arraySize) {
                char *checkLeft;
                char *checkRight;

                if(!containsChar(lines[i], ':') || lines[i][0] == ';') {
                    j++;
                    continue;
                }
                checkLeft = calloc(1, sizeof(char) * strlen(lines[j]) + 500);
                checkRight = calloc(1, sizeof(char) * strlen(lines[j]) + 500);

                splitContentLine(lines[j],checkLeft,checkRight);

                if(strcasecmp(checkLeft,"BEGIN") == 0 && strcasecmp(checkRight,"VEVENT") == 0) {
                    deallocator(checkLeft);
                    deallocator(checkRight);
                    deallocator(left);
                    deallocator(right);
                    return INV_EVENT;
                }

                if(strcasecmp(checkLeft,"END") == 0 && strcasecmp(checkRight,"VEVENT") == 0) {
                    i = j;
                    endFound = 1;
                    deallocator(checkLeft);
                    deallocator(checkRight);
                    break;
                }
                j++;
                deallocator(checkLeft);
                deallocator(checkRight);
            }

            if(endFound == 1) {
                deallocator(left);
                deallocator(right);
                continue;
            } else {
                deallocator(left);
                deallocator(right);
                return INV_EVENT;
            }
        }
    beginFound = 0;
    deallocator(left);
    deallocator(right);
    }
    return OK;
}



ICalErrorCode checkEventRequirements(char **lines, int arraySize) {
    int i;
    int openEvent = 0;
    int openOther = 0;
    int uidCount = 0;
    int dtStartCount = 0;
    int dtStampCount = 0;

    char *left;
    char *right;
    if(lines == NULL || arraySize == 0) {
        return OTHER_ERROR;
    }

    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i],':') || lines[i][0] == ';') {
            continue;
        }

        left = calloc(1, sizeof(char) * (strlen(lines[i])) + 100);
        right = calloc(1, sizeof(char) * (strlen(lines[i])) + 100);

        splitContentLine(lines[i],left,right);

        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right,"VEVENT") == 0) {
            openEvent++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"BEGIN") == 0 && openEvent == 1 && strcasecmp(right,"VALARM") == 0) {
            openOther++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "END") == 0 && openEvent == 1 && openOther >= 1 && strcasecmp(right,"VALARM") == 0) {
            openOther--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VEVENT") == 0) {
            openEvent--;

            if(openOther >= 1) {
                deallocator(left);
                deallocator(right);
                return INV_ALARM;
            }
            if(uidCount != 1 || dtStartCount != 1 || dtStampCount != 1) {
                deallocator(left);
                deallocator(right);
                return INV_EVENT;
            }
            uidCount = 0;
            dtStampCount = 0;
            dtStartCount = 0;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"UID") == 0 && openEvent == 1 && !openOther) {
            uidCount++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if (strcasecmp(left, "DTSTART") == 0 && openEvent == 1 && !openOther) {
            dtStartCount++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "DTSTAMP") == 0 && openEvent == 1 && !openOther) {
            dtStampCount++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        deallocator(left);
        deallocator(right);
    }

    return OK;

}

ICalErrorCode checkAlarmBeginEnd(char **lines, int arraySize) {
    int i;
    int j;
    int eventOpen = 0;
    int isFound = 0;
    char *right;
    char *left;
    if(lines == NULL || arraySize == 0) {
        return OTHER_ERROR;
    }

    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i], ':') || lines[i][0] == ';') {
            continue;
        }

        left = calloc(1, sizeof(char) * (strlen(lines[i])) + 100);
        right = calloc(1, sizeof(char) * (strlen(lines[i])) + 100);

        splitContentLine(lines[i], left,right);



        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right,"VEVENT") == 0) {
            eventOpen++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "END") == 0 && strcasecmp(right, "VEVENT") == 0) {
            eventOpen--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VALARM") == 0 && !isFound) {
            deallocator(left);
            deallocator(right);
            return INV_EVENT;
        }


        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right, "VALARM") == 0) {
            if(eventOpen != 1) {
                deallocator(left);
                deallocator(right);
                return INV_ALARM;
            }

            j = i+1;

            while(j < arraySize) {
                char *tempLeft;
                char *tempRight;

                if(!containsChar(lines[j],':') || lines[j][0] == ';') {
                    j++;
                    continue;
                }

                tempLeft = calloc(1, sizeof(char) * (strlen(lines[j])) + 100);
                tempRight = calloc(1, sizeof(char) * (strlen(lines[j])) + 100);

                splitContentLine(lines[j], tempLeft,tempRight);

                if((strcasecmp(tempLeft,"BEGIN") == 0) && (strcasecmp(tempRight,"VALARM") == 0 || strcasecmp(tempRight,"VEVENT") == 0)) {
                    deallocator(tempLeft);
                    deallocator(tempRight);
                    deallocator(left);
                    deallocator(right);
                    return INV_ALARM;
                }

                if(strcasecmp(tempLeft, "END") == 0 && (strcasecmp(tempRight, "VEVENT") == 0)) {
                    deallocator(tempLeft);
                    deallocator(tempRight);
                    deallocator(left);
                    deallocator(right);
                    return INV_ALARM;
                }

                if(strcasecmp(tempLeft,"END") == 0 && strcasecmp(tempRight,"VALARM") == 0) {
                    i = j;
                    isFound = 1;
                    deallocator(tempLeft);
                    deallocator(tempRight);
                    deallocator(left);
                    deallocator(right);
                    break;
                }
                j++;
                deallocator(tempLeft);
                deallocator(tempRight);
            }

            if(isFound == 1) {
                isFound = 0;
                continue;
            }

            deallocator(left);
            deallocator(right);
        }



        deallocator(left);
        deallocator(right);

    }

    return OK;

}


ICalErrorCode checkAlarmRequirements(char ** lines, int arraySize) {
    int i;
    int openEvent = 0;
    int openAlarm = 0;
    int triggerCount = 0;
    int actionCount = 0;
    char *right;
    char *left;
    if(lines == NULL || arraySize == 0) {
        return OTHER_ERROR;
    }

    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i], ':') || lines[i][0] == ';') {
            continue;
        }

        left = calloc(1, sizeof(char) * (strlen(lines[i])) + 100);
        right = calloc(1, sizeof(char) * (strlen(lines[i])) + 100);

        splitContentLine(lines[i], left,right);


        if(strcasecmp(left, "BEGIN") == 0 && strcasecmp(right, "VEVENT") == 0) {
            openEvent++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "END") == 0 && strcasecmp(right, "VEVENT") == 0) {
            openEvent--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "BEGIN") == 0 && strcasecmp(right, "VALARM") == 0 && openEvent == 1) {
            openAlarm++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "END") == 0 && strcasecmp(right, "VALARM") == 0 && openEvent == 1) {
            openAlarm--;
            if(triggerCount != 1 || actionCount != 1) {
                deallocator(left);
                deallocator(right);
                return INV_ALARM;
            }
            triggerCount = 0;
            actionCount = 0;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left , "TRIGGER") == 0 && openAlarm == 1 && openEvent == 1) {
            triggerCount++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "ACTION") == 0 && openAlarm == 1 && openEvent == 1) {
            actionCount++;
            deallocator(left);
            deallocator(right);
            continue;
        }
        deallocator(left);
        deallocator(right);
    }

    return OK;
}


ICalErrorCode checkEventHead(char **lines, int arraySize) {
    int i;
    char *right;
    char *left;
    int openEvent = 0;
    int openAlarm = 0;
    int uidCount = 0;
    int dtStart = 0;
    int dtStamp = 0;
    if(lines == NULL || arraySize == 0) {
        return INV_FILE;
    }
    //Only need to check the lines between the first and last lines
    //It has been confirmed that the basic calendar components have been confirmed
    for(i = 0;i<arraySize;i++) {
        /* Do the regular split of the left and right, once you are in an event you can check for the required
        fields, such as UID, DTSTART and the DTSTAMP */

        if(!containsChar(lines[i],':') || lines[i][0] == ';') {
            continue;
        }
        left = calloc(1,sizeof(char) * strlen(lines[i]) +100);
        right = calloc(1,sizeof(char) * strlen(lines[i]) + 100);
        /* The string contains the char */

        //splitByFirstOccurence(lines[i],left,right,':');

        if(containsChar(lines[i], ';') && checkBefore(lines[i],';',':')) {
            /* We are going to split by first occurence of the ; */
            splitByFirstOccurence(lines[i], left,right,';');
        } else {
            /* we are going to split by first occurence of the : */
            splitByFirstOccurence(lines[i], left,right,':');
        }

        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right,"VEVENT") == 0 && openAlarm == 0) {
            openEvent++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right,"VALARM") == 0) {
            openAlarm++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VEVENT") == 0 && openAlarm == 0) {
            if(uidCount != 1 || dtStart != 1 || dtStamp != 1 || openEvent != 1) {
                deallocator(left);
                deallocator(right);
                //printf("This was called\n");
                return INV_EVENT;
            }
            uidCount = 0;
            dtStart = 0;
            dtStamp = 0;
            openAlarm = 0;
            openEvent--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VALARM") == 0 && openAlarm == 1) {
            openAlarm--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"UID") == 0 && openEvent == 1 && openAlarm == 0) {
            uidCount++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"DTSTART") == 0 && openEvent == 1 && openAlarm == 0) {
            dtStart++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"DTSTAMP") == 0 && openEvent == 1 && openAlarm == 0) {
            dtStamp++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        deallocator(left);
        deallocator(right);

    } // end loop

    if(openEvent != 0) {
        return INV_EVENT;
    }
    return OK;
}

ICalErrorCode checkAlarmHead(char **lines, int arraySize) {
    int openAlarm = 0;
    int actionCount = 0;
    int triggerCount = 0;
    int i;
    char *left;
    char *right;

    if(lines == NULL || arraySize == 0) {
        return INV_FILE;
    }

    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i],':') || lines[i][0]) {
            continue;
        }
        left = calloc(1,sizeof(char) * strlen(lines[i]) +100);
        right = calloc(1,sizeof(char) * strlen(lines[i]) + 100);
        /* The string contains the char */

        //splitByFirstOccurence(lines[i],left,right,':');

        if(containsChar(lines[i], ';') && checkBefore(lines[i],';',':')) {
            /* We are going to split by first occurence of the ; */
            splitByFirstOccurence(lines[i], left,right,';');
        } else {
            /* we are going to split by first occurence of the : */
            splitByFirstOccurence(lines[i], left,right,':');
        }

        /* Here just check for the alarms required props */


        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right,"VALARM") == 0) {
            openAlarm++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"ACTION") == 0 && openAlarm == 1) {
            if(right == NULL  || strlen(right) == 0) {
                deallocator(left);
                deallocator(right);
                return INV_ALARM;
            }
            actionCount++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "TRIGGER") == 0 && openAlarm == 1) {
            if(right == NULL || strlen(right) == 0) {
                deallocator(left);
                deallocator(right);
                return INV_ALARM;
            }
            triggerCount++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VALARM") == 0) {
            if(triggerCount == 1 || actionCount == 1 || openAlarm == 1) {
                triggerCount = 0;
                actionCount = 0;
                openAlarm--;
                deallocator(left);
                deallocator(right);
                continue;
            }
                deallocator(left);
                deallocator(right);
                return INV_ALARM;

        }
        deallocator(left);
        deallocator(right);
    }

    if(openAlarm != 0) {
        return INV_ALARM;
    }

    return OK;
}

ICalErrorCode fetchCalendarProps(Calendar * obj,char **lines,int arraySize) {
    int i;
    int open = 0;
    char *left;
    char *right;

    Property *new_prop = NULL;

    if(lines == NULL || obj == NULL || arraySize == 0) {
        return OTHER_ERROR;
    }

    for(i = 0;i<arraySize;i++) {
        if(lines[i][0] == ';') {
            continue;
        }

        if(!containsChar(lines[i],':') && lines[i][0] != ';') {
            if(open == 1) {
                return INV_CAL;
            }
        }

        left = calloc(1,sizeof(char) * strlen(lines[i]) +100);
        right = calloc(1,sizeof(char) * strlen(lines[i]) + 100);
        /* The string contains the char */


        splitContentLine(lines[i], left,right);


        if((isEmpty(left) || isEmpty(right)) && open == 1) {
                D;
                deallocator(left);
                deallocator(right);
                return INV_CAL;
            }

        if(strcasecmp(left,"BEGIN") == 0 && (strcasecmp(right,"VEVENT") == 0 || strcasecmp(right,"VALARM") == 0 || strcasecmp(right,"VCALENDAR") == 0)) {
            open++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && (strcasecmp(right,"VEVENT") == 0 || strcasecmp(right,"VALARM") == 0 || strcasecmp(right,"VCALENDAR") == 0)) {
            open--;
            deallocator(left);
            deallocator(right);
            continue;
        }


        /* These are the properties that belong to the calendar! */
        if(open == 1) {
            new_prop = malloc(sizeof(Property) * strlen(left) + 300);
            if(strcasecmp(left, "VERSION") == 0) { /* This needs to be fixed */
                int len;
                float ignore;
                int ret = sscanf(right,"%f %n", &ignore,&len);
                if(ret==1 && !right[len]) {
                    obj->version = ignore;
                    deallocator(new_prop);
                } else {
                    deallocator(left);
                    deallocator(right);
                    free(new_prop);
                    return INV_VER;
                }
                //obj->version = atof(right);
            }else if(strcasecmp(left,"PRODID") == 0) {
                if(strlen(right) != 0) {
                    strcpy(obj->prodID,right);
                    deallocator(new_prop);
                } else {
                    deallocator(left);
                    deallocator(right);
                    free(new_prop);
                    return INV_PRODID;
                }
            } else {
                if(isEmpty(right) || isEmpty(left)) {
                    deallocator(left);
                    deallocator(right);
                    free(new_prop);
                    return INV_CAL;
                }
                strcpy(new_prop->propName,left);
                strcpy(new_prop->propDescr,right);
                insertBack(obj->properties, new_prop);
            }
        }

        deallocator(left);
        deallocator(right);
    }
    return OK;
}

ICalErrorCode fetchCalEvents(Calendar *obj, char **lines,int arraySize) {
    int i;
    int calOpen = 0;
    int eventOpen = 0;
    int alarmOpen = 0;
    int tzid = 0;
    char *left;
    char *right;

    Event *new_event = NULL;
    Alarm *new_alarm = NULL;
    Property *newAlarmProp = NULL;
    Property *newEventProp = NULL;
    List *eventPropList = NULL;
    List *alarmList = NULL;
    List *alarmProps = NULL;

    if(lines == NULL || obj == NULL || arraySize == 0) {
        return OTHER_ERROR;
    }

    for(i = 0;i<arraySize;i++) {
        //Ignoring the comments
        if(lines[i][0] == ';') {
            continue;
        }
        if(!containsChar(lines[i],':')) {
            if(calOpen == 1 && eventOpen == 1 && !alarmOpen && lines[i][0] != ';') {
                return INV_EVENT;
            }
            // if(calOpen == 1 && eventOpen == 1 && alarmOpen == 1 && lines[i][0] != ';') {
            //     return INV_ALARM;
            // }
        }
        left = calloc(1,sizeof(char) * strlen(lines[i]) + 500);
        right = calloc(1,sizeof(char) * strlen(lines[i]) + 500);
        /* The string contains the char */

        splitContentLine(lines[i],left,right);

        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right,"VCALENDAR") == 0) {
            calOpen++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VCALENDAR") == 0) {
            calOpen--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right,"VEVENT") == 0 && calOpen == 1) {
            new_event = malloc(sizeof(Event));
            eventPropList = initializeList(&printProperty,&deleteProperty,&compareProperties);
            alarmList = initializeList(&printAlarm,&deleteAlarm,&compareAlarms);
            /* For the purpose of the trigger not being null */
            eventOpen++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VEVENT") == 0 && calOpen == 1) {
            eventOpen--;
            deallocator(left);
            deallocator(right);
            new_event->alarms = alarmList;
            new_event->properties = eventPropList;
            insertBack(obj->events,new_event);
            alarmList = NULL;
            eventPropList = NULL;
            continue;
        }

        if(strcasecmp(left,"BEGIN") == 0 && strcasecmp(right,"VALARM") == 0 && eventOpen == 1 && calOpen == 1) {
            new_alarm = malloc(sizeof(Alarm));
            alarmProps = initializeList(&printProperty, &deleteProperty,&compareProperties);
            new_alarm->trigger = calloc(1, sizeof(char) * 500);
            alarmOpen++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VALARM") == 0 && alarmOpen == 1 && eventOpen == 1 && calOpen == 1) {
            new_alarm->properties = alarmProps;
            insertBack(alarmList, new_alarm);
            alarmOpen--;
            deallocator(left);
            deallocator(right);
            alarmProps = NULL;
            continue;
        }

        /* Getting the event */
        if(calOpen == 1 && eventOpen == 1 && !alarmOpen) {
            //printf("An Event property\n");

            if(strcasecmp(left,"UID") == 0 && new_event != NULL) {
                if(isEmpty(right)) {
                    deallocator(left);
                    deallocator(right);
                    freeList(eventPropList);
                    freeList(alarmList);
                    free(new_event);
                    return INV_EVENT;
                }
                strcpy(new_event->UID,right);
            } else if(strcasecmp(left,"DTSTART") == 0 && new_event != NULL) {
                if(containsChar(right,':')) {
                    char *tempLeft = calloc(1,sizeof(char) * strlen(left) + 100);
                    char *tempRight = calloc(1, sizeof(char) * strlen(right) + 100);
                    splitByFirstOccurence(right,tempLeft,tempRight,':');
                    strcpy(right,tempRight);
                    strcpy(left,tempLeft);
                    deallocator(tempLeft);
                    deallocator(tempRight);
                }



                if(isEmpty(right)) {
                    deallocator(left);
                    deallocator(right);
                    freeList(eventPropList);
                    freeList(alarmList);
                    free(new_event);
                    return INV_DT;
                }

                /* Make a comntains substring here that will look for the TZID */

                if(containsSubstring(left,"TZID")) {
                    tzid = 1;
                }


                if(containsChar(right,'T')) {//If there is a local time, or UTC
                    /*Take right and split */


                    char *date = calloc(1,sizeof(char) * 500);
                    char *time = calloc(1, sizeof(char) * 500);

                    splitByFirstOccurence(right,date,time,'T');

                    if(isEmpty(time) || tzid) {
                        deallocator(date);
                        deallocator(time);
                        deallocator(left);
                        deallocator(right);
                        freeList(eventPropList);
                        freeList(alarmList);
                        free(new_event);
                        return INV_DT;
                    }


                    if(containsChar(time,'Z')) {
                        if(strlen(time) != 7) {
                            deallocator(date);
                            deallocator(time);
                            deallocator(left);
                            deallocator(right);
                            freeList(eventPropList);
                            freeList(alarmList);
                            free(new_event);
                            return INV_DT;
                        }

                        
                        time[strlen(time) - 1] = '\0';
                        new_event->startDateTime.UTC = true;
                    } else {
                        if(strlen(time) != 6) {
                            deallocator(date);
                            deallocator(time);
                            deallocator(left);
                            deallocator(right);
                            freeList(eventPropList);
                            freeList(alarmList);
                            free(new_event);
                            return INV_DT;
                        }

                        new_event->startDateTime.UTC = false;
                    }
                    strcpy(new_event->startDateTime.date, date);
                    strcpy(new_event->startDateTime.time, time);
                    deallocator(date);
                    deallocator(time);
                } else {

                    deallocator(left);
                    deallocator(right);
                    freeList(eventPropList);
                    freeList(alarmList);
                    free(new_event);
                    return INV_DT;
                    // strcpy(new_event->startDateTime.date,right);
                    // strcpy(new_event->startDateTime.time,"NONE");
                    // new_event->startDateTime.UTC = false;
                }
            }else if(strcasecmp(left,"DTSTAMP") == 0 && new_event != NULL) {
                if(containsChar(right,':')) {
                    char *tempLeft = calloc(1,sizeof(char) * strlen(left) + 100);
                    char *tempRight = calloc(1, sizeof(char) * strlen(right) + 100);
                    splitByFirstOccurence(right,tempLeft,tempRight,':');
                    strcpy(right,tempRight);
                    strcpy(left,tempLeft);
                    deallocator(tempRight);
                    deallocator(tempLeft);
                }
                if(isEmpty(right)) {
                    deallocator(left);
                    deallocator(right);
                    freeList(eventPropList);
                    freeList(alarmList);
                    free(new_event);
                    return INV_DT;
                }


                if(containsSubstring(left,"TZID")) {
                    tzid = 1;
                }

                if(containsChar(right,'T')) {//If there is a local time, or UTC
                    /*Take right and split */
                    char *date = calloc(1,sizeof(char) * 500);
                    char *time = calloc(1, sizeof(char)* 500);
                    splitByFirstOccurence(right,date,time,'T');

                    if(isEmpty(time) || tzid) {
                        deallocator(date);
                        deallocator(time);
                        deallocator(left);
                        deallocator(right);
                        freeList(eventPropList);
                        freeList(alarmList);
                        free(new_event);
                        return INV_DT;
                    }
        
                    if(containsChar(time,'Z')) {
                        if(strlen(time) != 7) {
                            deallocator(date);
                            deallocator(time);
                            deallocator(left);
                            deallocator(right);
                            freeList(eventPropList);
                            freeList(alarmList);
                            free(new_event);
                            return INV_DT;
                        }

                        if(strlen(date) != 8) {
                            deallocator(date);
                            deallocator(time);
                            deallocator(left);
                            deallocator(right);
                            freeList(eventPropList);
                            freeList(alarmList);
                            free(new_event);
                            return INV_DT;
                        }
                        new_event->creationDateTime.UTC = true;
                        time[strlen(time) - 1] = '\0';
                    } else {
                        if(strlen(time) != 6) {
                            deallocator(date);
                            deallocator(time);
                            deallocator(left);
                            deallocator(right);
                            freeList(eventPropList);
                            freeList(alarmList);
                            free(new_event);
                            return INV_DT;
                        }

                        // if(strlen(date) != 7) {
                        //     deallocator(date);
                        //     deallocator(time);
                        //     deallocator(left);
                        //     deallocator(right);
                        //     freeList(eventPropList);
                        //     freeList(alarmList);
                        //     free(new_event);
                        //     return INV_DT;
                        // }
                        new_event->creationDateTime.UTC = false;
                    }
                    strcpy(new_event->creationDateTime.date, date);
                    strcpy(new_event->creationDateTime.time, time);
                    deallocator(date);
                    deallocator(time);
                } else {
                    deallocator(left);
                    deallocator(right);
                    freeList(eventPropList);
                    freeList(alarmList);
                    free(new_event);
                    return INV_DT;
                }
            } else {
                newEventProp = malloc(sizeof(Property) * strlen(right) + 200);
                // if(isEmpty(right) || isEmpty(left)) {
                //     deallocator(left);
                //     deallocator(right);
                //     freeList(alarmList);
                //     freeList(eventPropList);
                //     free(newEventProp);
                //     free(new_event);
                //     return INV_EVENT;
                // }
                if(!isEmpty(left))strcpy(newEventProp->propName, left);
                if(!isEmpty(right))strcpy(newEventProp->propDescr,right);
                insertBack(eventPropList,newEventProp);

            }

            /* I still have to get all of the properties of the event */
        }

        /* Getting the Alarm */
        if(calOpen == 1 && eventOpen == 1 && alarmOpen == 1) {
            /* The properties that are in the alarm comp */
            //printf("LEFT:%s\tRIGHT:%s\n",left,right);

            if(isEmpty(left) || isEmpty(right)) {
                deallocator(left);
                deallocator(right);
                freeList(eventPropList);
                freeList(alarmList);
                free(new_event);
                return INV_ALARM;
            }
            if(strcasecmp(left,"ACTION") == 0) {
                if(isEmpty(right)) {
                    deallocator(left);
                    deallocator(right);
                    free(new_alarm->trigger);
                    free(new_alarm);
                    free(new_event);
                    freeList(alarmList);
                    freeList(alarmProps);
                    freeList(eventPropList);
                    return INV_ALARM;
                }
                strcpy(new_alarm->action,right);
            }else if(strcasecmp(left,"TRIGGER") == 0) {
                if(isEmpty(right)) {
                    deallocator(left);
                    deallocator(right);
                    free(new_alarm->trigger);
                    free(new_alarm);
                    free(new_event);
                    freeList(alarmList);
                    freeList(alarmProps);
                    freeList(eventPropList);
                    return INV_ALARM;
                }
                /* The trigger was initialized in the top to restrict it from being null */
                //new_alarm->trigger = calloc(1, sizeof(char) * 500);
                strcpy(new_alarm->trigger, right);
            } else {
                newAlarmProp = malloc(sizeof(Property) * strlen(left) + 300);
                //printf("right:%s\n", right);
                if(isEmpty(right) || isEmpty(left)) {
                    deallocator(left);
                    deallocator(right);
                    freeList(alarmProps);
                    freeList(alarmList);
                    freeList(eventPropList);
                    free(new_alarm->trigger);
                    free(new_alarm);
                    free(new_event);
                    free(newAlarmProp);
                    return INV_ALARM;
                }
                strcpy(newAlarmProp->propName,left);
                strcpy(newAlarmProp->propDescr,right);
                insertBack(alarmProps,newAlarmProp);
            }
        }
        deallocator(left);
        deallocator(right);

    }
    return OK;
}


/* Ending the functions that help with parsing the linked list*/

/* Starting the mandatory functions for the assignment */

ICalErrorCode createCalendar(char* fileName, Calendar** obj) { //Big mem leak fix on the tokenizer
    /*First step, when opening the file make sure it is of valid file extension
    Also make sure that the actual file opens and that you can read contents from the file
    */
    ICalErrorCode error;
    int i;
    int arraySize;
    int contentSize;
    int fileLines;
    char **test = NULL;
    *obj = malloc(sizeof(Calendar));
    /* These cannot be NULL but can be empty */

    /* START OF FILE FUNCTIONS */
    error = validateFile(fileName);

    if(error != 0) {
        free(*obj);
        *obj = NULL;
        return INV_FILE;
    }

    /* End OF FILE FUNCTIONS */

    /* This method has been fixed */
    test = readFileChar(fileName, &arraySize,&fileLines);

    if(test == NULL) {
        free(*obj);
        *obj = NULL;
        return INV_FILE;
    }


    /* This function has been fixed */
    error = validateFileLines(test,arraySize,fileLines);

    if(error != 0) { //Error With the file
        free_fields(test,arraySize);
        // deallocator(obj);
        free(*obj);
        *obj = NULL;
        return error;
    }

    //Remove all of the special chars on each  line
    for(i = 0;i<arraySize;i++) {
        test[i] = trimSpecialChars(test[i]);
    }


    /* Check if a line is completely empty, then do invalid calendar */

    /* What we should do is return a new char ** that includes the unfolded lines */

    char **contentLines = lineUnfold(test,arraySize,&contentSize);

    if(contentLines == NULL) {
        free(*obj);
        *obj = NULL;
        return OTHER_ERROR;
    }


    /* Check if each line has a colon and/or semi colon */


    error = checkCalendarBeginEnd(contentLines,contentSize);
    if(error != 0) {
        free_fields(contentLines, contentSize);
        free(*obj);
        *obj = NULL;
        return error;
    }


    error = checkCalendarLayer(contentLines, contentSize);
    if(error != 0) {
        free_fields(contentLines, contentSize);
        free(*obj);
        *obj = NULL;
        return error;
    }


    //END OF TESTING

    error = checkEventBeginEnd(contentLines, contentSize);

    if(error != 0) {
        free_fields(contentLines, contentSize);
        free(*obj);
        *obj = NULL;
        return error;
    }


    error = checkEventRequirements(contentLines, contentSize);
    if(error != 0) {
        free_fields(contentLines, contentSize);
        free(*obj);
        *obj = NULL;
        return error;
    }


    error = checkAlarmBeginEnd(contentLines, contentSize);

    if(error != 0) {
        free_fields(contentLines, contentSize);
        free(*obj);
        *obj = NULL;
        return error;
    }


    error = checkAlarmRequirements(contentLines, contentSize);

    if(error != 0) {
        free_fields(contentLines, contentSize);
        free(*obj);
        *obj = NULL;
        return error;
    }

    (*obj)->events = initializeList(&printEvent,&deleteEvent,&compareEvents);
    (*obj)->properties = initializeList(&printProperty,&deleteProperty,&compareProperties);

    // /* Make functions to return the version and proID into the calendar object */

    // //(*obj)->properties = initializeList(&printProperty,&deleteProperty,&compareProperties);
    error = fetchCalendarProps(*obj,contentLines, contentSize);
    if(error != 0) {
        free_fields(contentLines, contentSize);
        // deallocator(obj);
        //FREE
        deleteCalendar(*obj);
        return error;
    }

    // // Look for the events
    // /* Basically look for the BEGIN:VEVENT then loop until you find the END:VEVENT. The parse all of the contents out of the VEVENT */
    // /* Just assume a simple CALENDAR file, and then from there just continue. I will check for the validations later*/


    // /* Check events */

    error = fetchCalEvents(*obj, contentLines, contentSize);
    if(error != 0) {
        free_fields(contentLines, contentSize);
        deleteCalendar(*obj);
        return error;
    }

    free_fields(contentLines, contentSize);

    return OK;
}


char *printCalendar(const Calendar *obj) {
    /* END TEST */


    char *strEvent = NULL;
    if(obj == NULL) {
        return NULL;
    }

    strEvent = calloc(1, sizeof(char) * 20);
    strcpy(strEvent, "Hello, World!\0");


    //printf("BEGIN CALENDAR\n");
    //printf("\tVERSION:%.2f\n", obj->version);
    //printf("\tPRODID:%s\n", obj->prodID);


    void *calProps;

    ListIterator calPropIter = createIterator(obj->properties);

    while((calProps = nextElement(&calPropIter)) != NULL) {
        Property *calProp = (Property*)calProps;
        char *strCalProp = obj->properties->printData(calProp);
        //printf("%s", strCalProp);
        deallocator(strCalProp);
    }



    /* FIND EVENTS AND PRINT THEM */
    void *event;
    /* HERE WE WILL GO THROUGH THE OBJS EVENTS */

    ListIterator eventIter = createIterator(obj->events);

    while((event = nextElement(&eventIter)) != NULL) {
        //printf("BEGIN EVENT:\n");
        Event *listEvent = (Event*)event;
        char *strEvent = obj->events->printData(listEvent);
        //printf("%s", strEvent);
        deallocator(strEvent);

        void *listAlarm;

        ListIterator alarmIter = createIterator(listEvent->alarms);

        while((listAlarm = nextElement(&alarmIter)) != NULL) {
           // printf("BEGIN ALARM:\n");
            Alarm *newAlarm = (Alarm*)listAlarm;
            char *strAlarm = listEvent->alarms->printData(newAlarm);
            //printf("%s", strAlarm);
            deallocator(strAlarm);

            void *alarmProps;
            ListIterator alarmPropsIter = createIterator(newAlarm->properties);

            while((alarmProps = nextElement(&alarmPropsIter)) != NULL) {
                Property *alarmProperty = (Property*)alarmProps;
                char *strAlarmProps = newAlarm->properties->printData(alarmProperty);
                //printf("%s", strAlarmProps);
                deallocator(strAlarmProps);
            }
            //printf("END ALARM:\n");
        }

        void *eventProps;
        ListIterator eventPropIter = createIterator(listEvent->properties);
        while((eventProps = nextElement(&eventPropIter)) != NULL) {
            Property *eventProperty = (Property*)eventProps;
            char *strProp = listEvent->properties->printData(eventProperty);
            //printf("%s", strProp);
            deallocator(strProp);
        }
       // printf("END EVENT:\n");
    }
    //printf("END CALENDAR\n");

    return strEvent;
}

/* Freeing all of the contents of the Calendar */
void deleteCalendar(Calendar *obj) {
    if(obj == NULL) {
        free(obj);
        obj = NULL;
        return;
    }

    freeList(obj->events); /* This calls the free Alarms as well */
    freeList(obj->properties);

    free(obj);
    obj = NULL;
}

/* Printing errors */

/* typedef enum ers {OK, INV_FILE, INV_CAL, INV_VER, DUP_VER, INV_PRODID, DUP_PRODID, INV_EVENT, INV_DT, INV_ALARM, WRITE_ERROR, OTHER_ERROR } ICalErrorCode;*/
char *printError(ICalErrorCode err) {
    char *temp = calloc(1,sizeof(char) * 30);
    switch(err) {
        case 0:
            strcpy(temp,"OK");
            break;
        case 1:
            strcpy(temp,"INV_FILE");
            break;
        case 2:
            strcpy(temp,"INV_CAL");
            break;
        case 3:
            strcpy(temp,"INV_VER");
            break;
        case 4:
            strcpy(temp,"DUP_VER");
            break;
        case 5:
            strcpy(temp,"INV_PRODID");
            break;
        case 6:
            strcpy(temp,"DUP_PRODID");
            break;
        case 7:
            strcpy(temp,"INV_EVENT");
            break;
        case 8:
            strcpy(temp,"INV_DT");
            break;
        case 9:
            strcpy(temp,"INV_ALARM");
            break;
        case 10:
            strcpy(temp,"WRITE_ERROR");
            break;
        default:
            strcpy(temp,"OTHER_ERROR");
    }
    printf("Error -> %s\n", temp);
    return temp;
}


/* This function will return the calendar object and write to a file, my guess is this could return write_error */
ICalErrorCode writeCalendar(char* fileName, const Calendar* obj) {
    if(fileName == NULL || isEmpty(fileName) || !containsSubstring(fileName, ".ics") || obj == NULL) {
        return WRITE_ERROR;
    }


    FILE *fp;
    fp = fopen(fileName, "w");

    if(fp == NULL) {
        return WRITE_ERROR;
    }
    fprintf(fp, "BEGIN:VCALENDAR\r\n");
    fprintf(fp, "VERSION:%.1f\r\n", obj->version);
    fprintf(fp, "PRODID:%s\r\n", obj->prodID);

    void *calProps;

    ListIterator calPropIter = createIterator(obj->properties);

    while((calProps = nextElement(&calPropIter)) != NULL) {
        Property *calProp = (Property*)calProps;
        char *strCalProp = obj->properties->printData(calProp);
        fprintf(fp, "%s",strCalProp);
        deallocator(strCalProp);
    }

        /* FIND EVENTS AND PRINT THEM */
    void *event;
    /* HERE WE WILL GO THROUGH THE OBJS EVENTS */

    ListIterator eventIter = createIterator(obj->events);

    while((event = nextElement(&eventIter)) != NULL) {
        fprintf(fp,"BEGIN:VEVENT\r\n");
        Event *listEvent = (Event*)event;
        char *strEvent = obj->events->printData(listEvent);
        fprintf(fp,"%s", strEvent);
        deallocator(strEvent);

        void *listAlarm;

        ListIterator alarmIter = createIterator(listEvent->alarms);

        while((listAlarm = nextElement(&alarmIter)) != NULL) {
            fprintf(fp,"BEGIN:VALARM\r\n");
            Alarm *newAlarm = (Alarm*)listAlarm;
            char *strAlarm = listEvent->alarms->printData(newAlarm);
            fprintf(fp,"%s", strAlarm);
            deallocator(strAlarm);

            void *alarmProps;
            ListIterator alarmPropsIter = createIterator(newAlarm->properties);

            while((alarmProps = nextElement(&alarmPropsIter)) != NULL) {
                Property *alarmProperty = (Property*)alarmProps;
                char *strAlarmProps = newAlarm->properties->printData(alarmProperty);
                fprintf(fp,"%s", strAlarmProps);
                deallocator(strAlarmProps);
            }
            fprintf(fp,"END:VALARM\r\n");
        }

        void *eventProps;
        ListIterator eventPropIter = createIterator(listEvent->properties);
        while((eventProps = nextElement(&eventPropIter)) != NULL) {
            Property *eventProperty = (Property*)eventProps;
            char *strProp = listEvent->properties->printData(eventProperty);
            fprintf(fp,"%s", strProp);
            deallocator(strProp);
        }
        fprintf(fp,"END:VEVENT\r\n");
    }

    fprintf(fp, "END:VCALENDAR\r\n");
    fclose(fp);
    return OK;
}

/* More validation of the calendar, including DT and properties */
ICalErrorCode validateCalendar(const Calendar* obj) {

    void *event;
    /* HERE WE WILL GO THROUGH THE OBJS EVENTS */

    ListIterator eventIter = createIterator(obj->events);

    while((event = nextElement(&eventIter)) != NULL) {
        Event *listEvent = (Event*)event;
        //char *strEvent = obj->events->printData(listEvent);
        void *eventProps;
        ListIterator eventPropIter = createIterator(listEvent->properties);
        while((eventProps = nextElement(&eventPropIter)) != NULL) {
            Property *eventProperty = (Property*)eventProps;
            //fprintf(fp,"%s", strProp);
            if(isEmpty(eventProperty->propName) || isEmpty(eventProperty->propDescr)) {
                return INV_EVENT;
            }
        }
        //fprintf(fp,"END:VEVENT\r\n");
    }

    return OK;
}

/* Ending the mandatory functions for the assignment */

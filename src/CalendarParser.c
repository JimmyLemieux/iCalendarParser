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
        sprintf(temp, "\tUID:%s\n",tempEvent->UID);
        //tempStr = realloc(tempStr, sizeof(char) * strlen(temp));
        strcat(tempStr,temp);
        deallocator(temp);
    }

    if(!isEmpty(tempEvent->creationDateTime.date)) {
        temp = calloc(1,sizeof(char) * strlen(tempEvent->creationDateTime.date) + 500);
        sprintf(temp,"\tDTSTAMPDATE:%s\n",tempEvent->creationDateTime.date);
        //tempStr = realloc(tempStr,sizeof(char) * strlen(temp));
        strcat(tempStr,temp);
        deallocator(temp);
    } 


    if(!isEmpty(tempEvent->creationDateTime.time)) {
        temp = calloc(1, sizeof(char) * strlen(tempEvent->creationDateTime.time) + 500);
        sprintf(temp,"\tDTSTAMPTIME:%s\n", tempEvent->creationDateTime.time);
        //tempStr = realloc(tempStr,sizeof(char) * strlen(temp));
        strcat(tempStr,temp);
        deallocator(temp);
    }

    temp = calloc(1, sizeof(char) * (500));
    sprintf(temp,"\tDTSTAMPUTC:%d\n",tempEvent->creationDateTime.UTC);
    //tempStr = realloc(tempStr, sizeof(char) * strlen(temp) + 1);
    strcat(tempStr, temp);
    deallocator(temp);


    if(!isEmpty(tempEvent->startDateTime.date)) {
        temp = calloc(1,sizeof(char) * strlen(tempEvent->startDateTime.date) + 500);
        sprintf(temp,"\tDTSTARTDATE:%s\n",tempEvent->startDateTime.date);
        //tempStr = realloc(tempStr,sizeof(char) * strlen(temp));
        strcat(tempStr,temp);
        deallocator(temp);
    } 


    if(!isEmpty(tempEvent->startDateTime.time)) {
        temp = calloc(1, sizeof(char) * strlen(tempEvent->startDateTime.time) + 500);
        sprintf(temp,"\tDTSTARTTIME:%s\n", tempEvent->startDateTime.time);
        //tempStr = realloc(tempStr,sizeof(char) * strlen(temp));
        strcat(tempStr,temp);
        deallocator(temp);
    }

	return tempStr;
}
/* You will have to traverse all of the properties and alarms of this event as well */ 
/* You will need to free the two list * inside the event object */ 
int compareEvents(const void *first, const void *second) {
	Event *event1;
	Event *event2;

	if(first == NULL || second == NULL) {
		return 0;
	}

	event1 = (Event *)first;
	event2 = (Event *)second;
	/* Check all of the components of the property are the same
	Which includes, UID, DTSTART, DTSTAMP, and alarms if there is one */

	if(strcmp(event1->UID,event2->UID) != 0) {
		return 0;
	}

	if(strcmp(event1->startDateTime.date, event2->startDateTime.date) != 0) {
		return 0;
	}

	if(strcmp(event1->startDateTime.time,event2->startDateTime.time) != 0) {
		return 0;
	}

    if(strcmp(event1->creationDateTime.date,event2->creationDateTime.date) != 0) {
        return 0;
    }

    if(strcmp(event1->creationDateTime.time,event2->creationDateTime.time) != 0) {
        return 0;
    }

	if(event1->startDateTime.UTC != event2->startDateTime.UTC) {
		return 0;
	}

    if(event1->creationDateTime.UTC != event2->creationDateTime.UTC) {
        return 0;
    }

    /* You will have to call the compare alarms function here in order to compare the alarms and its properties */


	return 1;
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
    sprintf(tempStr, "\tACTION:%s\n\tTRIGGER:%s\n", tempAlarm->action, tempAlarm->trigger);
    return tempStr;
}

int compareAlarms(const void *first, const void *second) {
    Alarm *alarm1;
    Alarm *alarm2;
    if(first == NULL || second == NULL) {
        return 0;
    }
    alarm1 = (Alarm*)first;
    alarm2 = (Alarm*)second;
    if(strcmp(alarm1->trigger,alarm2->trigger) != 0) {
        return 0;
    }
    if(strcmp(alarm1->action,alarm2->action) != 0) {
        return 0;
    }
    /* You are going to have to comapre all of the properties inside of the alarm as well */

    /* You can just call compare properties here for each of the alarms */

    return 1;
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
    int len;
    Property *tempProp;
    if(toBePrinted == NULL) {
        return NULL;
    }

    tempProp = (Property *)toBePrinted;

    len = strlen(tempProp->propDescr) + strlen(tempProp->propName) + 500;

    str = calloc(1, len * sizeof(char));

    sprintf(str, "\t%s:%s\n", tempProp->propName,tempProp->propDescr);

    return str;
}

int compareProperties(const void *first, const void *second) {
    Property *prop1;
    Property *prop2;
    if(first == NULL || second == NULL) {
        return 0;
    }

    prop1 = (Property *)first;
    prop2 = (Property *)second;

    if(strcmp(prop1->propDescr,prop2->propDescr) != 0) {
        return 0;
    }

    if(strcmp(prop1->propName,prop2->propName) != 0) {
        return 0;
    }
    return 1;
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
    DateTime *date1;
    DateTime *date2;
    int isGood = 0;
    if(first == NULL || second == NULL) {
        return 0;
    }

    date1 = (DateTime*)first;
    date2 = (DateTime*)second;

    /* Need to compare the values of the date, time, and the UTC */

    if(strcmp(date1->date,date2->date) == 0) {
        isGood++;
    }

    if(strcmp(date1->time,date2->time) == 0) {
        isGood++;
    }

    if(date1->UTC == date2->UTC) {
        isGood++;
    }

    if(isGood == 3) {
        return 1;
    }
    return 0;
}

void deleteDate(void *toBeDeleted) {
    if(toBeDeleted == NULL) {
        return;
    }
    /* I am not sure what this function does tbh */


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
        printf("There is no file extension\n");
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
            printf("The file is not a ical file\n"); 
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


    /* This reads in the file line by line */


    /* Check to see if there is anything to read in the file */

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
        D;
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
    int calComp = 0;
    int open = 0;
    char *left = NULL;
    char *right = NULL;

    if(lines == NULL || arraySize == 0) {
        printf("Something is still wrong with the file\n");
        return INV_FILE;
    }
    //Check to see if the CALENDAR has a BEGIN and then END, IF THERE is at least ONE COMPONENT
    //CHECK IF THE CALENDAR has a version and a PROID

    /* Checking if the first and last lines are valid */

    // stringToLower(lines[0]);
    // stringToLower(lines[arraySize - 1]);

    if(strcasecmp(lines[0],"begin:vcalendar") != 0 || strcasecmp(lines[arraySize - 1], "end:vcalendar") != 0) {
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
        left = calloc(1,(index+1) * sizeof(left));
        right = calloc(1,(strlen(lines[i]) - index) * sizeof(char));
        for(k = 0;k<index;k++) {
            left[k] = lines[i][k];
        }
        //Since we have now found the right line, we can now find the right
        for(k = index+1,j=0;k<strlen(lines[i]);k++,j++) {
            right[j] = lines[i][k];
        }

        /*Change the left and right to lowerCase */
        // stringToLower(left); 
        // stringToLower(right);

        if((strcasecmp(left,"begin") == 0 || strcasecmp(left,"end") == 0) && (strcasecmp(right,"vcalendar") == 0)) {
            printf("There is a duplicate property in the file\n");
            deallocator(left);
            deallocator(right);
            return INV_CAL;
        }
        deallocator(left);
        deallocator(right);
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

        left = calloc(1, (index+1) * sizeof(char));
        right = calloc(1,(strlen(lines[i]) - index) * sizeof(char));


        for(k = 0;k<index;k++) {
            left[k] = lines[i][k];
        }

        for(k = index+1,j=0;k<strlen(lines[i]);k++,j++) {
            right[j] = lines[i][k];
        }    

        /* Set the right and left to be lowercase */
        // stringToLower(left);
        // stringToLower(right);

        if(open < 0) {
            printf("THERE WAS A BEGIN END MISMATCH");
            deallocator((char *)left);
            deallocator((char *)right);
            return INV_CAL;
        }

        if(strcasecmp(left,"begin") == 0) {
            if(strcasecmp(right,"VEVENT") == 0) {
                calComp++;
            }
            open++;
            deallocator((char *)left);
            deallocator((char *)right);
            continue; // to the next line
        }

        if(strcasecmp(left,"end") == 0) {
            open--;
            deallocator((char *)left);
            deallocator((char *)right);
            continue;
        }
        
        
        if(strcasecmp(left,"version") == 0 && open == 1) {
            if(!foundVersion) {
                foundVersion = 1;
            } else {
                deallocator((char *)left);
                deallocator((char *)right);
                printf("Dupe Version\n");
                return DUP_VER;
            }
        }

        if(strcasecmp(left,"prodid") == 0 && open == 1) {
            if(!foundPRODID) {
                foundPRODID = 1;
            } else {
                deallocator((char *)left);
                deallocator((char *)right); 
                printf("Dupe prodid\n");
                return DUP_PRODID;
            }
        }

        deallocator((char *)right);
        deallocator((char *)left);
    }


    if(!foundPRODID || !foundVersion || !calComp) {
        printf("Could't find prodid or version\n");
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
        return OTHER_ERROR;
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
                D;
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

        //splitByFirstOccurence(lines[i],left,right,':');
        splitContentLine(lines[i],left,right);

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VEVENT") == 0 && !beginFound) {
            deallocator(left);
            deallocator(right);
            return INV_EVENT;
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

        if(strcasecmp(left,"BEGIN") == 0 && openEvent == 1) {
            openOther++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left, "END") == 0 && openEvent == 1 && openOther >= 1) {
            openOther--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0 && strcasecmp(right,"VEVENT") == 0) {
            openEvent--;

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
            return INV_ALARM;
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


//This function is responsible for checking if there is at least one component in the CALENDAR
//If you can find a BEGIN:EVENT in the top level of the CALENDAR component this will return OK
//AN EVENT MUST have a BEGIN:VENEVT and then a END:VEVENT tag
//INSIDE an event it MUST have a DTSTAMP property and UID property and DTSTART property

//Will finish this function later, need to first check for mismatches of BEGIN and END
//This function will check if all of the events in the file are good


/* Check if the event contains the basic properties UID,DTSTART,DTSTAMP */
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
        printf("This is an invalid file\n");
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
                printf("This was called\n");
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
        printf("This is an invalid file\n");
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

/*This function will go through the properties that will be used with the calendar*/
/*This includes the required PRODID and VERSION, as well as any other property on the top level of the iCal */
/* Now adding the required functionality for parsing the "other props" */

ICalErrorCode fetchCalendarProps(Calendar * obj,char **lines,int arraySize) {
    int i;
    int open = 0;
    char *left;
    char *right;

    Property *new_prop = NULL;

    for(i = 0;i<arraySize;i++) {
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

        // stringToUpper(left);
        // stringToUpper(right);

        if(strcasecmp(left,"BEGIN") == 0) {
            open++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcasecmp(left,"END") == 0) {
            open--;
            deallocator(left);
            deallocator(right);
            continue;
        }


        /* These are the properties that belong to the calendar! */
        if(open == 1) {
            //printf("left:%s\tright:%s\n",left,right);
            new_prop = malloc(sizeof(Property));
            if(strcasecmp(left, "VERSION") == 0) {
                if(sscanf(right,"%f", &(obj->version)) != 0 && strlen(right) != 0) {
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
                strcpy(new_prop->propName,left);
                strcpy(new_prop->propDescr,right);
                if(isEmpty(right)) {
                    deallocator(left);
                    deallocator(right);
                    free(new_prop);
                    return INV_CAL;
                }
                insertBack(obj->properties, new_prop);
            }
        }

        deallocator(left);
        deallocator(right);
    }
    return OK;
}


/* There will need to be checking for alarms in the events as well */
/* It should be noted that events are not required to have alarms */
/* This function could make a sub call to another function that will parse the alarms out of the function*/
/* This function will also look for alarms */
ICalErrorCode fetchCalEvents(Calendar *obj, char **lines,int arraySize) {
    int i;
    int calOpen = 0;
    int eventOpen = 0;
    int alarmOpen = 0;
    char *left;
    char *right;

    Event *new_event = NULL;
    Alarm *new_alarm = NULL;
    Property *new_alarm_prop = NULL;
    Property *newEventProp = NULL;
    List *eventPropList = NULL;
    List *alarmList = NULL;
    List *alarmProps = NULL;

    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i],':') ||  lines[i][0] == ';') {
            continue;
        }
        left = calloc(1,sizeof(char) * strlen(lines[i]) + 500);
        right = calloc(1,sizeof(char) * strlen(lines[i]) + 500);
        /* The string contains the char */

        //splitByFirstOccurence(lines[i],left,right,':');

        if(containsChar(lines[i], ';') && checkBefore(lines[i],';',':')) {
            /* We are going to split by first occurence of the ; */
            splitByFirstOccurence(lines[i], left,right,';');

        } else {
            /* we are going to split by first occurence of the : */
            splitByFirstOccurence(lines[i], left,right,':');
        }

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
       

                if(containsChar(right,'T')) {//If there is a local time, or UTC
                    /*Take right and split */
                    char *date = calloc(1,sizeof(char) * 500);
                    char *time = calloc(1, sizeof(char) * 500);

                    splitByFirstOccurence(right,date,time,'T');
                    if(containsChar(time,'Z')) {
                        time[strlen(time) - 1] = '\0';
                        new_event->startDateTime.UTC = true;
                    } else {
                        new_event->startDateTime.UTC = false;
                    }
                    strcpy(new_event->startDateTime.date, date);
                    strcpy(new_event->startDateTime.time, time);
                    deallocator(date);
                    deallocator(time);
                } else {
                    strcpy(new_event->startDateTime.date,right);
                    strcpy(new_event->startDateTime.time,"NONE");
                    new_event->startDateTime.UTC = false;
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




                if(containsChar(right,'T')) {//If there is a local time, or UTC
                    /*Take right and split */
                    char *date = calloc(1,sizeof(char) * 500);
                    char *time = calloc(1, sizeof(char)* 500);
                    splitByFirstOccurence(right,date,time,'T');
                    if(containsChar(time,'Z')) {
                        new_event->creationDateTime.UTC = true;
                        time[strlen(time) - 1] = '\0';
                    } else {
                        new_event->creationDateTime.UTC = false;
                        //time[strlen(time) - 1] = '\0';
                    }
                    // printf("The length of the dateS is %lu\n", strlen(date));
                    // printf("The length of the timeS is %lu\n", strlen(time));
                    strcpy(new_event->creationDateTime.date, date);
                    strcpy(new_event->creationDateTime.time, time);
                    deallocator(date);
                    deallocator(time);
                } else {
                    strcpy(new_event->creationDateTime.date,right);
                    strcpy(new_event->creationDateTime.time,"NONE");
                    new_event->creationDateTime.UTC = false;
                    deallocator(left);
                    deallocator(right);
                }
            } else {
                newEventProp = malloc(sizeof(Property));
                strcpy(newEventProp->propName, left);
                strcpy(newEventProp->propDescr,right);
                if(isEmpty(right)) {
                    deallocator(left);
                    deallocator(right);
                    free(newEventProp); 
                    free(new_event);
                    freeList(eventPropList);
                    freeList(alarmList);
                    return INV_EVENT;
                }
                insertBack(eventPropList,newEventProp);

            }

            /* I still have to get all of the properties of the event */ 



        }

        /* Getting the Alarm */
        if(calOpen == 1 && eventOpen == 1 && alarmOpen == 1) {
            /* The properties that are in the alarm comp */
            //printf("LEFT:%s\tRIGHT:%s\n",left,right);
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
                new_alarm_prop = malloc(sizeof(Property));
                //printf("right:%s\n", right);
                if(isEmpty(right)) {
                    deallocator(left);
                    deallocator(right);
                    free(new_alarm);
                    free(new_alarm_prop);
                }
                strcpy(new_alarm_prop->propName,left);
                strcpy(new_alarm_prop->propDescr,right);
                insertBack(alarmProps,new_alarm_prop);
            }
        }
        deallocator(left);
        deallocator(right);

    }

    return OK;
}

/* As of right now, this function is kind of useless */
ICalErrorCode fetchCalAlarms(Calendar *obj, char **lines, int arraySize) {
    int i;
    int calOpen = 0;
    int eventOpen = 0;
    int alarmOpen = 0;
    char *left;
    char *right;
    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i],':') || lines[i][0] == ';') {
            continue;
        }
        left = calloc(1,sizeof(char) * strlen(lines[i]) +50);
        right = calloc(1,sizeof(char) * strlen(lines[i]) + 50);
        /* The string contains the char */

        //splitByFirstOccurence(lines[i],left,right,':');

        if(containsChar(lines[i], ';') && checkBefore(lines[i],';',':')) {
            /* We are going to split by first occurence of the ; */
            splitByFirstOccurence(lines[i], left,right,';');

        } else {
            /* we are going to split by first occurence of the : */
            splitByFirstOccurence(lines[i], left,right,':');
        }

        stringToUpper(left);
        stringToUpper(right);

        if(strcmp(left,"BEGIN") == 0 && strcmp(right,"VCALENDAR") == 0) {
            calOpen++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcmp(left,"END") == 0 && strcmp(right,"VCALENDAR") == 0) {
            calOpen--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcmp(left,"BEGIN") == 0 && strcmp(right,"VEVENT") == 0) {
            eventOpen++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcmp(left,"END") == 0 && strcmp(right,"VEVENT") == 0) {
            eventOpen--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcmp(left,"BEGIN") == 0 && strcmp(right,"VALARM") == 0) {
            alarmOpen++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcmp(left,"END") == 0 && strcmp(right,"VALARM") == 0) {
            alarmOpen--;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(calOpen == 1 && eventOpen == 1 && alarmOpen == 1) {
        }
        deallocator(left);
        deallocator(right);
    }
    return OK;

}




/* I still have to implement this function here as well */
void lineUnfold(char **lines, int arraySize) { 
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
    int fileLines;
   // obj = malloc(sizeof(Calendar*));
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
    char **test = readFileChar(fileName, &arraySize,&fileLines);

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

    /* Test valid prop names */

    error = checkBeginsAndEnds(test, arraySize);

    if(error != 0) {
        free_fields(test, arraySize);
        free(*obj);
        *obj = NULL;
        return error;
    }
    

    error = checkCalendarHead(test,arraySize);

    if(error != 0) {
        free_fields(test,arraySize);
        //FREE
        free(*obj);
        *obj = NULL;
        return error;
    }


    error = checkEventBeginEnd(test,arraySize);

    if(error != 0) {
        free_fields(test,arraySize);
        free(*obj);
        *obj = NULL;
        return error;
    }

    error = checkEventRequirements(test, arraySize);

    if(error != 0) {
        free_fields(test, arraySize);
        free(*obj);
        *obj = NULL;
        return error;
    }


    error = checkAlarmBeginEnd(test, arraySize);

    if(error != 0) {
        free_fields(test,arraySize);
        free(*obj);
        *obj = NULL;
        return error;
    }

    error = checkAlarmRequirements(test, arraySize);

    if(error != 0) {
        free_fields(test,arraySize);
        free(*obj);
        *obj = NULL;
        return error;
    }

    (*obj)->events = initializeList(&printEvent,&deleteEvent,&compareEvents);
    (*obj)->properties = initializeList(&printProperty,&deleteProperty,&compareProperties);

    // /* Make functions to return the version and proID into the calendar object */

    // //(*obj)->properties = initializeList(&printProperty,&deleteProperty,&compareProperties);
    error = fetchCalendarProps(*obj,test,arraySize);
    if(error != 0) {
        free_fields(test,arraySize);
        // deallocator(obj);
        //FREE
        deleteCalendar(*obj);
        return error;
    }

    // // Look for the events
    // /* Basically look for the BEGIN:VEVENT then loop until you find the END:VEVENT. The parse all of the contents out of the VEVENT */
    // /* Just assume a simple CALENDAR file, and then from there just continue. I will check for the validations later*/


    // /* Check events */

    error = fetchCalEvents(*obj, test,arraySize);
    if(error != 0) {
        free_fields(test,arraySize);
        deleteCalendar(*obj);
        return error;
    }

    free_fields(test,arraySize);

    return OK;
}


char *printCalendar(const Calendar *obj) {    
    /* END TEST */
    char *strEvent = NULL;
    printf("BEGIN CALENDAR\n");
    printf("\tVERSION:%.2f\n", obj->version);
    printf("\tPRODID:%s\n", obj->prodID);


    void *calProps;

    ListIterator calPropIter = createIterator(obj->properties);

    while((calProps = nextElement(&calPropIter)) != NULL) {
        Property *calProp = (Property*)calProps;
        char *strCalProp = obj->properties->printData(calProp);
        printf("%s", strCalProp);
        deallocator(strCalProp);
    }



    /* FIND EVENTS AND PRINT THEM */
    void *event;
    /* HERE WE WILL GO THROUGH THE OBJS EVENTS */

    ListIterator eventIter = createIterator(obj->events);

    while((event = nextElement(&eventIter)) != NULL) {
        printf("BEGIN EVENT:\n");
        Event *listEvent = (Event*)event;
        char *strEvent = obj->events->printData(listEvent);
        printf("%s", strEvent);
        deallocator(strEvent);

        void *listAlarm;

        ListIterator alarmIter = createIterator(listEvent->alarms);

        while((listAlarm = nextElement(&alarmIter)) != NULL) {
            printf("BEGIN ALARM:\n");
            Alarm *newAlarm = (Alarm*)listAlarm;
            char *strAlarm = listEvent->alarms->printData(newAlarm);
            printf("%s", strAlarm);
            deallocator(strAlarm);

            void *alarmProps;
            ListIterator alarmPropsIter = createIterator(newAlarm->properties);
            
            while((alarmProps = nextElement(&alarmPropsIter)) != NULL) {
                Property *alarmProperty = (Property*)alarmProps;
                char *strAlarmProps = newAlarm->properties->printData(alarmProperty);
                printf("%s", strAlarmProps);
                deallocator(strAlarmProps);
            }
            printf("END ALARM:\n");
        }




        void *eventProps;
        ListIterator eventPropIter = createIterator(listEvent->properties);
        while((eventProps = nextElement(&eventPropIter)) != NULL) {
            Property *eventProperty = (Property*)eventProps;
            char *strProp = listEvent->properties->printData(eventProperty);
            printf("%s", strProp);
            deallocator(strProp);
        }
        printf("END EVENT:\n");
    }   
    printf("END CALENDAR\n");
    
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
    return temp;
}

ICalErrorCode writeCalendar(char* fileName, const Calendar* obj) {
    return OK;
}

ICalErrorCode validateCalendar(const Calendar* obj) {
    return OK;
}

/* Ending the mandatory functions for the assignment */

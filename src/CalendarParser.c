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

	Event *tempEvent;
	if(toBePrinted == NULL) {
		return NULL;
	}

	tempEvent = (Event*)toBePrinted;

	/* We are going to have the print out the contents of the event object we just refrenced */
	// printf("%s\n",tempEvent->UID);
    int len = strlen(tempEvent->UID) + strlen(tempEvent->startDateTime.date) + strlen(tempEvent->creationDateTime.date) + strlen(tempEvent->creationDateTime.time) + 70;
	tempStr = calloc(1, len);
	sprintf(tempStr, "UID: %s, Start Time is %s\n The Creation date is %s, The Creation time is %s\n", tempEvent->UID,tempEvent->startDateTime.date,tempEvent->creationDateTime.date,tempEvent->creationDateTime.time); 
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

	if(event1->startDateTime.UTC != event2->startDateTime.UTC) {
		return 0;
	}
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
    freeList(tempEvent->alarms);
}

char *printAlarm(void *toBePrinted) {
    char *tempStr;
    Alarm *tempAlarm;
    int len;
    if(toBePrinted == NULL) {
        return NULL;
    }
    tempAlarm = (Alarm *)toBePrinted;
    len = strlen(tempAlarm->action) + strlen(tempAlarm->trigger) + 90;
    tempStr = calloc(1, sizeof(char) * len);
    sprintf(tempStr, "Alarms Action is : %s, Alarms Trigger is : %s\n", tempAlarm->action, tempAlarm->trigger);
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
    /* I am also going to have to look through all of the properties */
    return 1;
}

void deleteAlarm(void *toBeDeleted) {
    Alarm *tempAlarm;

    if(toBeDeleted == NULL) {
        return;
    }
    tempAlarm = (Alarm*)toBeDeleted;
    /* I will also have to go through the properties for this and free */
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

    len = strlen(tempProp->propDescr) + strlen(tempProp->propName) + 90;

    str = calloc(1, len * sizeof(char));

    sprintf(str, "Property name:%s\nProperty Desc:%s\n", tempProp->propName,tempProp->propDescr);

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

/* Starting helper functions for basic file checking */



/* End the functions for basic file checking */

ICalErrorCode validateFile(char *fileName) {
    FILE *file;
    int errnum;
    int j;
    int index;
    char *tempFile = NULL;
    char *fileExtension = NULL;

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
        errnum = errno;
        printf("There was an error on the file load\n");
        fprintf(stderr,"Error opening file: %s\n",strerror(errnum));
        deallocator(tempFile);
        deallocator(fileExtension);
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
void unfoldLines(char **lines, int arraySize) {
    int i;
    char **tempLines;
    if(lines == NULL || arraySize == 0) {
        printf("There are no lines to unfold!\n");
        return;
    }

    tempLines = (char **)malloc(sizeof(char *) * arraySize);
    for(i = 0;i<arraySize;i++) {
        /* Any line that is not the first that has a space at the front is a potential line fold
        Also any line that has a strlen of zero should be removed from the list */ 
        // if(isspace(lines[i][0])) {
        //     printf("Potential Line fold on line : %s\n", lines[i]);
        // }
        tempLines[i] = calloc(1,sizeof(char) * strlen(lines[i]) + 1);
        strcpy(tempLines[i], lines[i]);
    }

    printf("Printing the temp array!\n");

    for(i = 0;i<arraySize;i++) {
        printf("%s\n",tempLines[i]);
    }


}


//This works with big input
char** readFileChar(char *fileName, int *arraySize,int *fileLines) { //Cool tokenizer and memleak fix
    int c;
    int fileLineCount = 0;
    int lineSize = 0;
    int stringSize = 0;
    int index = 0;
    int tempStart = 0;
    int k = 0;
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


    *fileLines = fileLineCount;
    *arraySize = lineSize;
    fclose(file);
    deallocator(temp);
    deallocator((char *)stringBuffer);
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
    char *left = NULL;
    char *right = NULL;

    if(lines == NULL || arraySize == 0) {
        printf("Something is still wrong with the file\n");
        return INV_FILE;
    }
    //Check to see if the CALENDAR has a BEGIN and then END, IF THERE is at least ONE COMPONENT
    //CHECK IF THE CALENDAR has a version and a PROID

    /* Checking if the first and last lines are valid */

    stringToLower(lines[0]);
    stringToLower(lines[arraySize - 1]);

    if(strcmp(lines[0],"begin:vcalendar") != 0 || strcmp(lines[arraySize - 1], "end:vcalendar") != 0) {
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
        stringToLower(left); 
        stringToLower(right);

        if((strcmp(left,"begin") == 0 || strcmp(left,"end") == 0) && (strcmp(right,"vcalendar") == 0)) {
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
        stringToLower(left);
        stringToLower(right);

        if(open < 0) {
            printf("THERE WAS A BEGIN END MISMATCH");
            deallocator((char *)left);
            deallocator((char *)right);
            return INV_CAL;
        }

        if(strcmp(left,"begin") == 0) {
            open++;
            deallocator((char *)left);
            deallocator((char *)right);
            continue; // to the next line
        }

        if(strcmp(left,"end") == 0) {
            open--;
            deallocator((char *)left);
            deallocator((char *)right);
            continue;
        }
        
        
        if(strcmp(left,"version") == 0 && open == 1) {
            if(!foundVersion) {
                foundVersion = 1;
            } else {
                deallocator((char *)left);
                deallocator((char *)right);
                return DUP_VER;
            }
        }

        if(strcmp(left,"prodid") == 0 && open == 1) {
            if(!foundPRODID) {
                foundPRODID = 1;
            } else {
                deallocator((char *)left);
                deallocator((char *)right); 
                return DUP_PRODID;
            }
        }
        deallocator((char *)right);
        deallocator((char *)left);
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

//Will finish this function later, need to first check for mismatches of BEGIN and END
//This function will check if all of the events in the file are good


//I should make a find first occurence function that will find the first occurence of a char and split to left and right
ICalErrorCode checkEvents(char **lines, int arraySize) {
    int i;
    int j;
    int k;
    int index;
    char *right;
    char *left;
    if(lines == NULL || arraySize == 0) {
        printf("This is an invalid file\n");
        return INV_FILE;
    }
    //Only need to check the lines between the first and last lines
    //It has been confirmed that the basic calendar components have been confirmed
    for(i = 1;i<arraySize - 1;i++) {
        index = 0;
        while(index < strlen(lines[i]) && lines[i][index] != ':') {
            index++;
        }

        if(index == strlen(lines[i])) {
            continue;
        }
        
        left = calloc(1, (index+1) * sizeof(char));
        right = calloc(1, ((strlen(lines[i]) - index)+1) * sizeof(char));

        for(k = 0;k<index;k++) {
            left[k] = lines[i][k];
        }

        for(k=index+1,j=0;k<strlen(lines[i]);k++,j++) {
            right[j] = lines[i][k];
        }
        /* chage the right and left to lowercase */
        /* This is for checking for case insensitive things in the properties */
        stringToLower(left);
        stringToLower(right);

        //Now that we have the right and left begin to look for the open and closed events 
        printf("LEFT:%s :::: RIGHT:%s\n",left,right);
    } // end loop
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

    Property *new_prop;
    List *props;

    new_prop = malloc(sizeof(Property));
    props = initializeList(&printProperty,&deleteProperty,&compareProperties);
    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i],':')) {
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

        if(strcmp(left,"BEGIN") == 0) {
            open++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcmp(left,"END") == 0) {
            open--;
            deallocator(left);
            deallocator(right);
            continue;
        }


        /* These are the properties that belong to the calendar! */
        if(open == 1) {
            //printf("left:%s\tright:%s\n",left,right);
            if(strcmp(left, "VERSION") == 0) {
                obj->version = atof(right);
            }else if(strcmp(left,"PRODID") == 0) {
                strcpy(obj->prodID,right);
            } else {
                strcpy(new_prop->propName,left);
                strcpy(new_prop->propDescr,right);
                insertBack(obj->properties, new_prop);
            }
        }

        deallocator(left);
        deallocator(right);
    }
    deallocator(new_prop);
    obj->properties = props;
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

    Event *new_event;
    List *eventList;

    eventList = initializeList(&printEvent,&deleteEvent,&compareEvents);
    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i],':')) {
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
            new_event = malloc(sizeof(Event));
            eventOpen++;
            deallocator(left);
            deallocator(right);
            continue;
        }

        if(strcmp(left,"END") == 0 && strcmp(right,"VEVENT") == 0) {
            eventOpen--;
            deallocator(left);
            deallocator(right);
            insertBack(eventList,new_event);
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

        if(calOpen == 1 && eventOpen == 1) {
            //printf("An Event property\n");
            if(strcmp(left,"UID") == 0 && new_event != NULL) {
                strcpy(new_event->UID,right);
            } 

            if(strcmp(left,"DTSTART") == 0 && new_event != NULL) {
                if(containsChar(right,'T')) {//If there is a local time, or UTC
                    /*Take right and split */
                    char *date = calloc(1,sizeof(char) * 50);
                    char *time = calloc(1, sizeof(char)* 50);

                    splitByFirstOccurence(right,date,time,'T');
                    if(containsChar(time,'Z')) {
                        new_event->startDateTime.UTC = true;
                        time[strlen(time) - 1] = '\0';
                    } else {
                        new_event->startDateTime.UTC = false;
                    }

                    strcpy(new_event->startDateTime.date, date);
                    strcpy(new_event->startDateTime.time, time);
                    deallocator(date);
                    deallocator(time);
                } else {
                    strcpy(new_event->startDateTime.date,right);
                    // deallocator(left);
                    // deallocator(right);
                }
            }

            if(strcmp(left,"DTSTAMP") == 0 && new_event != NULL) {
                if(containsChar(right,'T')) {//If there is a local time, or UTC
                    /*Take right and split */
                    char *date = calloc(1,sizeof(char) * 50);
                    char *time = calloc(1, sizeof(char)* 50);

                    splitByFirstOccurence(right,date,time,'T');
                    if(containsChar(time,'Z')) {
                        new_event->creationDateTime.UTC = true;
                        time[strlen(time) - 1] = '\0';
                    } else {
                        new_event->creationDateTime.UTC = false;
                    }
                    strcpy(new_event->creationDateTime.date, date);
                    strcpy(new_event->creationDateTime.time, time);
                    deallocator(date);
                    deallocator(time);
                } else {
                    strcpy(new_event->creationDateTime.date,right);
                    // deallocator(left);
                    // deallocator(right);
                }
            }
        }


        if(calOpen == 1 && eventOpen == 1 && alarmOpen == 1) {
            /* The properties that are in the alarm comp */
            //printf("LEFT:%s\tRIGHT:%s\n",left,right);

        }
        deallocator(left);
        deallocator(right);

    }

    obj->events = eventList;
    return OK;
}

ICalErrorCode fetchCalAlarms(Calendar *obj, char **lines, int arraySize) {
    int i;
    int calOpen = 0;
    int eventOpen = 0;
    int alarmOpen = 0;
    char *left;
    char *right;
    for(i = 0;i<arraySize;i++) {
        if(!containsChar(lines[i],':')) {
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
    obj = malloc(sizeof(Calendar*));
    *obj = malloc(sizeof(Calendar));


    /* START OF FILE FUNCTIONS */
    error = validateFile(fileName);

    if(error != 0) {
        free(*obj);
        free(obj);
        // free((Calendar*)(*obj));
        // free((Calendar**)obj);
        return INV_FILE;
    }

    /* End OF FILE FUNCTIONS */

    /* This method has been fixed */
    char **test = readFileChar(fileName, &arraySize,&fileLines);
   

    /* This function has been fixed */
    error = validateFileLines(test,arraySize,fileLines); 
    if(error != 0) { //Error With the file
        printf("Invalid file\n");
        free_fields(test,arraySize);
        free(*obj);
        free(obj);
        return INV_FILE;
    }


    //If there is a pass, continue to look at the calendar contents
    // The calendar contents are supposed to be specified inside the text

    
    printf("\\THIS FILE WAS FLAGGED AS VALID\\\n");

    //Remove all of the special chars on each  line
    for(i = 0;i<arraySize;i++) {
        test[i] = trimSpecialChars(test[i]);
    }
    //unfoldLines(test,arraySize);

    error = checkCalendarHead(test,arraySize);

    if(error != 0) {
        printf("This is an invalid calendar\n");
        free_fields(test,arraySize);
        free(*obj);
        free(obj);
        return INV_FILE;
    }



    /* Check for events and other things later, start the parser for now */
    printf("\\THIS IS A GOOD CALENDAR FILE!!\\\n");
    printf("NOW PARSING THE CALENDAR CONTENTS!\n\n\n");

    /* Make functions to return the version and proID into the calendar object */

    //(*obj)->properties = initializeList(&printProperty,&deleteProperty,&compareProperties);
    error = fetchCalendarProps(*obj,test,arraySize);

    if(error != 0) {
        printf("Found an error while parsing the version and proID\n");
        free_fields(test,arraySize);
        free(*obj);
        free(obj);
        return OTHER_ERROR;
    }

    // Look for the events
    /* Basically look for the BEGIN:VEVENT then loop until you find the END:VEVENT. The parse all of the contents out of the VEVENT */
    /* Just assume a simple CALENDAR file, and then from there just continue. I will check for the validations later*/

    error = fetchCalEvents(*obj, test,arraySize);

    if(error != 0) {
        printf("Found an error while looking for the events\n");
        free_fields(test,arraySize);
        free(*obj);
        free(obj);
        return OTHER_ERROR;
    }

    error = fetchCalAlarms(*obj, test,arraySize);

    if(error != 0) {
        printf("Found an error while looking for the events\n");
        free_fields(test,arraySize);
        free(*obj);
        free(obj);
        return OTHER_ERROR;
    }
    printf("%.2f\n",(*obj)->version);
    printf("%s\n", (*obj)->prodID);
    printf("List has been freed!\n");
    free_fields(test,arraySize);
    freeList((*obj)->events);
    freeList((*obj)->properties);
    free(*obj);
    free(obj); 
    return OK;
}
/* Ending the mandatory functions for the assignment */

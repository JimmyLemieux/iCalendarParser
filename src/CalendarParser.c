#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include "LinkedListAPI.h"
#include "CalendarParser.h"
#include "helper.c"
#define D printf("debug\n")
#define R printf("Check for Return\n")
// #include "LinkedListAPI.h"

//Add a MISMATCH CHECKER that will check BEGIN and END tags


Calendar cal;
List list;

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
	free(tempEvent);
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
    free(tempAlarm->trigger);
    free(tempAlarm);
}

/*Ending the functions for the linked list */








/* Starting the helper functions for parsing the calendar  */

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

    printf("%s\n", lines[0]);
    printf("%s\n", lines[arraySize - 1]); 

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
            free(left);
            free(right);
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
            free(right);
            free(left);
            return INV_CAL;
        }

        if(strcmp(left,"begin") == 0) {
            open++;
            free(right);
            free(left);
            continue; // to the next line
        }

        if(strcmp(left,"end") == 0) {
            open--;
            free(right);
            free(left);
            continue;
        }
        
        
        if(strcmp(left,"version") == 0 && open == 1) {
            if(!foundVersion) {
                foundVersion = 1;
            } else {
                free(left);
                free(right);
                return DUP_VER;
            }
        }

        if(strcmp(left,"prodid") == 0 && open == 1) {
            if(!foundPRODID) {
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
        right = calloc(1, (strlen(lines[i]) - index) * sizeof(char));

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
        free(left);
        free(right);
    } // end loop
    return OK;
}


/*This function will go through the properties that will be used with the calendar*/
/*This includes the required PRODID and VERSION, as well as any other property on the top level of the iCal */
/* Now adding the required functionality for parsing the "other props" */

ICalErrorCode fetchCalRequiredProps(Calendar * obj,char **lines,int arraySize) {
    //The object needs to be malloced 
    // All of the things have been checked, then we can just look for the version and proID
    int i;
    int j;
    int k;
    int index;
    char *left;
    char *right;
    if(lines == NULL || obj == NULL) {
        return OTHER_ERROR;
    }

    for(i = 0;i<arraySize;i++) {
        index = 0;
        while(index < strlen(lines[i]) && lines[i][index] != ':') {
            index++;
        }

        if(index == strlen(lines[i])) {
            printf("The token was not found on this line\n");
            continue;
        }

        /* If the : is found, then we need to make a right and left */
        left = calloc(1, (index+1) *sizeof(char));
        right = calloc(1, (strlen(lines[i]) - index) * sizeof(char));
        for(j = 0;j<index;j++) {
            left[j] = lines[i][j]; 
        }

        for(j=index+1,k=0;j<strlen(lines[i]);j++,k++) {
            right[k] = lines[i][j];
        }

        /* The right and left need to converted into lower */
        stringToLower(left);
        stringToLower(right);


        if(strcmp(left,"version") == 0) { // This is a version that should be parsed, when it is parsed add to the calendar object
            obj->version = atof(right);
            free(right);
            free(left);
            continue;
        }

        if(strcmp(left,"prodid") == 0) {
            strcpy(obj->prodID,right);
            free(left);
            free(right);
            continue;
        }
        
        free(left);
        free(right);

    }
    return OK;
}


/* There will need to be checking for alarms in the events as well */
/* It should be noted that events are not required to have alarms */
/* This function could make a sub call to another function that will parse the alarms out of the function*/
/* This function will also look for alarms */
ICalErrorCode fetchCalEvents(Calendar *obj, char **lines,int arraySize) {
    int i;
    int j;
    int k;
    int index;
    int open = 0;
    int alarmOpen = 0;
    char *left;
    char *right;
    Event *new_event;
    Alarm *new_alarm;
    List *eventList;
    List *alarmList;

    if(obj == NULL || lines == NULL) {
        return OTHER_ERROR;
    }

    /* Initialize the list that will store all of the events that have been parsed */
    /* The list needs to have the required functions that will be used to compare,print and delete */

    eventList = initializeList(&printEvent,&deleteEvent,&compareEvents);
    
    if(eventList == NULL) {
        printf("The list was not init properly!\n");
        return OTHER_ERROR;
    } 


    /* Loop through all of the calendar events and parse all of the contents */
    /* Assume that all of the contents of the event are proper and in order */
    /* I need to make a check for the events later */


    for(i = 0;i<arraySize;i++) {
        index = 0;
        while(index < strlen(lines[i]) && lines[i][index] != ':') {
            index++;
        }
        if(index == strlen(lines[i])) {
            continue;
        }

        /* The character was found then we have to see if the conponent is a VEVENT*/

        left = calloc(1, (index+1) * sizeof(char));
        right = calloc(1, (strlen(lines[i]) - index) * sizeof(char));

        /* Populate the left and right char * */

        for(j = 0;j<index;j++) {
            left[j] = lines[i][j];
        }
         
        for(j = index+1,k=0;j<strlen(lines[i]);j++,k++) {
            right[k] = lines[i][j];
        }

        /* change the right and left to lowercase */
        stringToLower(left);
        stringToLower(right);

        /* Check if there is an opening for the event */
        if(strcmp(left,"begin") == 0 && strcmp(right,"vevent") == 0) {
            open++;
            new_event = malloc(sizeof(Event));
            alarmList = initializeList(&printAlarm,&deleteAlarm,&compareAlarms);
            free(right);
            free(left);
            continue;
        }

        if(strcmp(left,"uid") == 0 && open) {
            strcpy(new_event->UID,right);
            free(right);
            free(left);
            continue;
        }


        if(strcmp(left,"dtstart") == 0 && open) { 
            /*Making a new Datetime structure */ 
            strcpy(new_event->startDateTime.date, right);
            free(right);
            free(left);
            continue;
        }


        if(strcmp(left, "dtstamp") == 0 && open) { //This finds an abort trap
            //This is where the abort trap
            char *leftStamp;
            char *rightStamp;

            leftStamp = calloc(1,30 * sizeof(char));
            rightStamp = calloc(1, 30 * sizeof(char));

            splitByFirstOccurence(right,leftStamp,rightStamp,'t');

            strcpy(new_event->creationDateTime.date, leftStamp);
            strcpy(new_event->creationDateTime.time, rightStamp);
            
            free(leftStamp);
            free(rightStamp);
            free(right);
            free(left);
            continue;
        }



        /*An alarm is usually located withing the event. This is assuming everything is fine with the calendar
        I am going to have to provide check functions that will check the quality of the list before parsing */

        /* If you find an alarm you should then loop through the REST of the lines until you find the end alarm tag */
        /* From that point you can begin the parsing */

        /* These need a trigger and an action */
        if(strcmp(left,"begin") == 0 && strcmp(right,"valarm") == 0 && open) {
            alarmOpen++;
            printf("Found where an alarm begins!\n");
            new_alarm = malloc(sizeof(Alarm));
            free(left);
            free(right);
            continue;
        }   

        if(strcmp(left,"trigger") == 0 && open && alarmOpen) {
            new_alarm->trigger = malloc(sizeof(char) * strlen(right)+1);
            stringToUpper(right);
            strcpy(new_alarm->trigger, right);
            //new_alarm->trigger = right;
            free(left);
            free(right);
            continue;
        }

        if(strcmp(left,"action") == 0 && open && alarmOpen) {
            stringToUpper(right);
            strcpy(new_alarm->action,right);
            free(left);
            free(right);
            continue;
        }

        if(strcmp(left,"end") == 0 && strcmp(right,"valarm") == 0 && open && alarmOpen) {
            alarmOpen--;
            printf("In an event with a UID of %s\n",new_event->UID);
            printf("The Trigger = %s\n", new_alarm->trigger);
            printf("The Action = %s\n",new_alarm->action);

            insertBack(alarmList, (void *)new_alarm);
            /* Instead this should be pushed onto the alarm list at the current event */
            // free(new_alarm->trigger);
            // free(new_alarm);
            free(left);
            free(right);
            continue;
        }

        /* There needs to be an end to the event in order for this to work */
        /* I am going to have to make a validation of mismatches to make sure that everything is fine */
        /* This is where the event should be added to the linked list*/
        /* Somehow pass the List into the function and push the list */ 
        if(strcmp(left,"end") == 0 && strcmp(right,"vevent") == 0) {
            open--;
            new_event->alarms = alarmList;                  //Each event block will have a list of alarms
            insertBack(eventList,(void *)new_event);        //This will be freed in the freelist
            free(right);
            free(left);
            continue;
        }


    free(right);
    free(left);
    }
    obj->events = eventList;
    
    return OK;
}

/* Ending the functions that help with parsing the linked list*/


/* Starting the mandatory functions for the assignment */

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
        printf("This is an invalid calendar\n");
        free_fields(test,arraySize);
        free(tempFile);
        free(fileExtension);
        return INV_FILE;
    }

    /* Check for events and other things later, start the parser for now */
    printf("\\THIS IS A GOOD CALENDAR FILE!!\\\n");

    /* Make functions to return the version and proID into the calendar object */

    error = fetchCalRequiredProps(*obj,test,arraySize);

    if(error != 0) {
        printf("Found an error while parsing the version and proID\n");
        free_fields(test,arraySize);
        free(tempFile); 
        free(fileExtension);
        return OTHER_ERROR;
    }

    printf("THE REQUIRED COMPONENETS OF THE CALENDAR\n\n");
    printf("----------------------------------------\n");
    printf("The version is %f\n", (*obj)->version);
    printf("The proID is %s\n\n\n",(*obj)->prodID);
    printf("----------------------------------------\n");

    // Look for the events
    /* Basically look for the BEGIN:VEVENT then loop until you find the END:VEVENT. The parse all of the contents out of the VEVENT */
    /* Just assume a simple CALENDAR file, and then from there just continue. I will check for the validations later*/

    error = fetchCalEvents(*obj, test,arraySize);

    if(error != 0) {
        printf("Found an error while looking for the events\n");
        free_fields(test,arraySize);
        free(tempFile);
        free(fileExtension);
        return OTHER_ERROR;
    }


    printf("Printing the events that are in the cal obj\n");
    void *elem;
    ListIterator iter = createIterator((*obj)->events);

    while((elem = nextElement(&iter)) != NULL) {
        Event *tmp = (Event*)elem;
        char *str = (*obj)->events->printData(tmp);
        printf("%s\n", str);
        Alarm *tempAlarm = getFromFront(tmp->alarms);
        printf("The first alarm in the event is:\n");
        printf("Action: %s\n", tempAlarm->action);
        printf("Trigger: %s\n" ,tempAlarm->trigger);
        printf("\n");
        free(str);
    }

    freeList((*obj)->events);
    printf("List has been freed!\n");

    free_fields(test,arraySize);
    free(tempFile);
    free(fileExtension);
    free(*obj);
    free(obj);
    return OK;
}


/* Ending the mandatory functions for the assignment */


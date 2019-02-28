#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "CalendarParser.h"

/* I am going to implement the dateTime helper functions */

int main(int argc, char **argv) {
    Calendar *obj;
    ICalErrorCode error;
    char *errorOut;
    

    int isDriver = 0;

    if(argc == 3) {
        if(strcasecmp(argv[2], "1") == 0) isDriver = 1;
    }

    //This will load from a file
    if(!isDriver) {
        printf("*****RUNNING THE CREATE CALENDAR FUNCTION*****\n");
        error = createCalendar(argv[1], &obj);
        printf("printing the status of create Calendar\n");
        errorOut = printError(error);
        free(errorOut);

        if(error != 0) {
            // The delete calendar would have already been called in the calendar parser funtion
            printf("ENDING THE CALENDAR PARSER FUNCTION\n");
            return 0;
        }

        error = validateCalendar(obj);
        printf("printing the status of validate calendar\n");
        errorOut = printError(error);

        writeCalendar("outFiles/testOut.ics", obj);
        deleteCalendar(obj);
        free(errorOut);
        return 0;
    }

    /* Manually making a calendar object */

    printf("*****USING THE MANUAL DRIVER*****\n");

    obj = malloc(sizeof(Calendar));

    obj->version = 2.0;
    strcpy(obj->prodID, "-//hacksw/h,andcal//NONSGML v1.0//EN");
    obj->events = initializeList(&printEvent,&deleteEvent,&compareEvents);

    obj->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);


    Property *newCalProp = malloc(sizeof(Property) +  5000);
    strcpy(newCalProp->propName, "CALSCALE");
    strcpy(newCalProp->propDescr, "AWESOME");
    insertBack(obj->properties, newCalProp);



    Event * newEvent = calloc(1, sizeof(Event));
    newEvent->alarms = initializeList(&printAlarm, &deleteAlarm,&compareAlarms);

    strcpy(newEvent->creationDateTime.date, "20090901"); 
    strcpy(newEvent->creationDateTime.time, "170000");
    newEvent->creationDateTime.UTC = 1;

    strcpy(newEvent->startDateTime.date, "20090901"); 
    strcpy(newEvent->startDateTime.time, "170000");
    newEvent->startDateTime.UTC = 0;

    strcpy(newEvent->UID, "SOME EVENT UID");
    newEvent->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);

    Property *newProp = malloc(sizeof(Property) + 5000);
    strcpy(newProp->propName, "UID");
    strcpy(newProp->propDescr, "4");
    insertBack(newEvent->properties, newProp);

    insertBack(obj->events, newEvent);


    Event * newEvent2 = calloc(1, sizeof(Event));
    newEvent2->alarms = initializeList(&printAlarm, &deleteAlarm,&compareAlarms);

    strcpy(newEvent2->creationDateTime.date, "20090901"); 
    strcpy(newEvent2->creationDateTime.time, "170000");
    newEvent2->creationDateTime.UTC = 1;

    strcpy(newEvent2->startDateTime.date, "20090901"); 
    strcpy(newEvent2->startDateTime.time, "170000");
    newEvent2->startDateTime.UTC = 1;

    strcpy(newEvent2->UID, "SOME EVENT UID");

    //Adding an alarm to the event

    Alarm *newAlarm = calloc(1, sizeof(Alarm));

    strcpy(newAlarm->action, "Some action");


    newAlarm->trigger = calloc(1, sizeof(char) * 200);
    strcpy(newAlarm->trigger, "There is some trigger here");
    newAlarm->properties = initializeList(&printAlarm, &deleteAlarm, &compareAlarms);


    insertBack(newEvent2->alarms,newAlarm); 




    Alarm *newAlarm2 = calloc(1, sizeof(Alarm));

    strcpy(newAlarm2->action, "New Alarm Action");
    newAlarm2->trigger = calloc(1, sizeof(char) * 100);
    strcpy(newAlarm2->trigger,"New Alarm Trigger");

    newAlarm2->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);

    Property *newAlarmProperty2 = calloc(1, sizeof(Property) + 200);

    strcpy(newAlarmProperty2->propName,"DURATION");
    strcpy(newAlarmProperty2->propDescr, "Duration");

    insertBack(newAlarm2->properties,newAlarmProperty2);


    Property *newAlarmProperty3 = calloc(1, sizeof(Property) + 200);

    strcpy(newAlarmProperty3->propName, "REPEAT");
    strcpy(newAlarmProperty3->propDescr, "Some Repeat");

    insertBack(newAlarm2->properties, newAlarmProperty3);

    insertBack(newEvent2->alarms, newAlarm2);

    newEvent2->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);


    Property *summaryProp = calloc(1, sizeof(Property) + 200);

    strcpy(summaryProp->propName, "SUMMARY");

    strcpy(summaryProp->propDescr, "THIS IS A SUMMARY TEST");


    insertBack(newEvent2->properties, summaryProp);

    //Add some properties to the event

    Property *eventProps = calloc(1, sizeof(Property) + 200);

    // This should be an error, There is a property that cannot appear more than once

    strcpy(eventProps->propName, "COMMENT");
    strcpy(eventProps->propDescr, "Some Description");


    Property *eventProps2 = calloc(1, sizeof(Property) + 200);

    strcpy(eventProps2->propName, "DTEND");
    strcpy(eventProps2->propDescr, "Some");

    insertBack(newEvent2->properties, eventProps2);


    insertBack(newEvent2->properties, eventProps);
    

    insertBack(obj->events, newEvent2);

    error = 0;


    if(error == 0) {

        error = validateCalendar(obj);


        errorOut = printError(error);

        free(errorOut);
        //printCalendar(obj);
        // if(argc != 3) {
        //     error = writeCalendar("outFiles/testOut.ics", obj);
        //     errorOut = printError(error);
        // } else {
        //     error = writeCalendar(argv[2], obj);
        //     errorOut = printError(error);
        // }
        // free(errorOut); 

        writeCalendar("outFiles/testOut.ics", obj);
        deleteCalendar(obj);
    }
    return 0;
}
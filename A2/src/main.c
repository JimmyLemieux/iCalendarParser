#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CalendarParser.h"

/* I am going to implement the dateTime helper functions */

int main(int argc, char **argv) {
    Calendar *obj;
    ICalErrorCode error;
    char *errorOut;
    

    // if(argc != 2) {
    //     error = createCalendar("test/sample1.ics", &obj);
    //     errorOut = printError(error);
    //     free(errorOut);
    // } else {
    //     error = createCalendar(argv[1], &obj);
    //     errorOut = printError(error);
    //     free(errorOut);
    // }

    /* Manually making a calendar object */

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
    strcpy(newProp->propName, "PRIORITY");
    strcpy(newProp->propDescr, "4");
    insertBack(newEvent->properties, newProp);

    Property *newProp2 = malloc(sizeof(Property) + 5000);
    strcpy(newProp2->propName, "SUMMARY");
    strcpy(newProp2->propDescr, "Nigga got hoes bitch nigga");
    insertBack(newEvent->properties, newProp2);


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
    newEvent2->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);


    //Add some properties to the event

    Property *eventProps = calloc(1, sizeof(Property) + 200);

    strcpy(eventProps->propName, "GEO");
    strcpy(eventProps->propDescr, "Some Description");


    Property *eventProps2 = calloc(1, sizeof(Property) + 200);

    strcpy(eventProps2->propName, "GEO");
    strcpy(eventProps2->propDescr, "Some Description");

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
        deleteCalendar(obj);
    }
    return 0;
}
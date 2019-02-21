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
    strcpy(obj->prodID, "-//hacksw/handcal//NONSGML v1.0//EN");
    obj->events = initializeList(&printEvent,&deleteEvent,&compareEvents);

    obj->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);


    Property *newCalProp = malloc(sizeof(Property) +  5000);
    strcpy(newCalProp->propName, "CALSCALE");
    strcpy(newCalProp->propDescr, "AWESOME");
    insertBack(obj->properties, newCalProp);


    // Property *newCalProp2 = malloc(sizeof(Property) + 5000);
    // strcpy(newCalProp2->propName, "DESCRIPTION");
    // strcpy(newCalProp2->propDescr, "THE DESC OF PROP");
    // insertBack(obj->properties, newCalProp2);



    Event * newEvent = calloc(1, sizeof(Event));
    newEvent->alarms = initializeList(&printAlarm, &deleteAlarm,&compareAlarms);

    strcpy(newEvent->creationDateTime.date, "20090901"); 
    strcpy(newEvent->creationDateTime.time, "170000");
    newEvent->creationDateTime.UTC = 1;

    strcpy(newEvent->startDateTime.date, "20090901"); 
    strcpy(newEvent->startDateTime.time, "170000");
    newEvent->startDateTime.UTC = 1;

    strcpy(newEvent->UID, "SOME EVENT UID");
    newEvent->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);

    Property *newProp = malloc(sizeof(Property) + 5000);
    strcpy(newProp->propName, "CLASS");
    strcpy(newProp->propDescr, "ISCOOL");
    insertBack(newEvent->properties, newProp);


    insertBack(obj->events, newEvent);

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
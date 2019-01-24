#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CalendarParser.h"
#define D printf("DEBUG\n");

/* I am going to implement the dateTime helper functions */

int main() {
    Calendar *obj;
    ICalErrorCode error;
    char * errorStr;
    char *outString;
    error = createCalendar("qqq.ics", &obj);

    if(error == 0) {
        printf("deleting the calendar\n");
        outString = printCalendar(obj);
        free(outString);
        // freeList(obj->events);
        // freeList(obj->properties); 
        deleteCalendar(obj);
    }
    //outString = printCalendar(obj);
    errorStr = printError(error); 
    printf("Error - > %s\n", errorStr);
    free(errorStr);

    //deleteCalendar(obj);
    return 0;
}

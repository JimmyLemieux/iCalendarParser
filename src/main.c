#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CalendarParser.h"

/* I am going to implement the dateTime helper functions */

int main() {
    Calendar *obj;
    ICalErrorCode error;
    char * errorStr = NULL;
    char *outString;
    error = createCalendar("garb.ics", &obj);

    errorStr = printError(error);

    printf("ERROR -> %s\n",errorStr);
    free(errorStr);
    if(error == 0) {
        printf("deleting the calendar\n");
        outString = printCalendar(obj);
        deleteCalendar(obj);
    }



    return 0;
}

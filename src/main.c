#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CalendarParser.h"

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
        deleteCalendar(obj);
    }

    return 0;
}

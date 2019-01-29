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
    error = createCalendar("qqq.ics", &obj);

    errorStr = printError(error);

    printf("ERROR -> %s\n",errorStr);
    free(errorStr);
    
    if(error == 0) {
        outString = printCalendar(obj);
        outString = NULL;
        deleteCalendar(obj);
    }
    return 0;
}

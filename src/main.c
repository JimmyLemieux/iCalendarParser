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
    error = createCalendar("sample1.ics", &obj);
    if(error == 0) {
        outString = printCalendar(obj);
        free(outString);
        deleteCalendar(obj);
    }
    //outString = printCalendar(obj);
    errorStr = printError(error); 
    printf("Error - > %s\n", errorStr);
    free(errorStr);
    //free(outString);

    //deleteCalendar(obj);
    return 0;
}

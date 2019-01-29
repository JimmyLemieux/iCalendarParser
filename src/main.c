#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CalendarParser.h"

/* I am going to implement the dateTime helper functions */

int main(int argc, char **argv) {
    Calendar *obj;
    ICalErrorCode error;
    char *errorOut;
    

    if(argc != 2) {
        error = createCalendar("sample1.ics", &obj);
        errorOut = printError(error);
    } else {
        error = createCalendar(argv[1], &obj);
        errorOut = printError(error);
    }
    
    if(error == 0) {
        printCalendar(obj);
        deleteCalendar(obj);
    }
    return 0;
}

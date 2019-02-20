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
        error = createCalendar("test/sample1.ics", &obj);
        errorOut = printError(error);
        free(errorOut);
    } else {
        error = createCalendar(argv[1], &obj);
        errorOut = printError(error);
        free(errorOut);
    }
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
    printf("NIGGA\n");
    return 0;
}
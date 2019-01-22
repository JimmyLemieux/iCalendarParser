#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CalendarParser.h"
#define D printf("DEBUG\n");

/* I am going to implement the dateTime helper functions */

int main() {
    Calendar *obj;
    ICalErrorCode error;
    char * outString;
    char * errorStr;
    error = createCalendar("test.ics", &obj);
    if(obj == NULL) {
        return 0;
    }
    outString = printCalendar(obj);




    errorStr = printError(error); 
    printf("%s\n", errorStr);

    free(errorStr);

    free(outString);

    return 0;
}

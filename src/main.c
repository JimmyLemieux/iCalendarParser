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
    error = createCalendar("sample1.ics", &obj);
    outString = printCalendar(obj);


    errorStr = printError(error); 
    printf("%s\n", errorStr);

    free(errorStr);

    if(outString != NULL){
        printf("%s",outString);
        free(outString);
    }


    deleteCalendar(obj);

    return 0;
}

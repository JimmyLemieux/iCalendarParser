#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CalendarParser.h"
#define D printf("DEBUG\n");

int main() {
    Calendar *obj;
    ICalErrorCode error;
    char * outString;
    error = createCalendar("sample1.ics", &obj);
    outString = printCalendar(obj);
    printf("%s\n", printError(error));


    if(outString != NULL){
        printf("%s",outString);
        free(outString);
    }


    deleteCalendar(obj);

    return 0;
}

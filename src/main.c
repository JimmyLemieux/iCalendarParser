#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CalendarParser.h"

int main() {
    Calendar *obj;
    ICalErrorCode error;
    char * outString;
    error = createCalendar("sample1.ics", &obj); 
    if(error == 0) {
        printf("OK!\n");
    }
    if(obj == NULL) {
        printf("The object is null!\n");
    }

    outString = printCalendar(obj);
    printf("%s",outString);
    free(outString);
    deleteCalendar(obj);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <ctype.h>
// #include "CalendarParser.h"
// #include "LinkedListAPI.h"
#include "CalendarParser.h"

int main() {
    createCalendar("test.ics", NULL); 
    //printf("Error Code: %u\n", createCalendar("badFile.ical", NULL));
    return 0;
}

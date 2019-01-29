#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CalendarParser.h"

/* I am going to implement the dateTime helper functions */

int main(int argc, char **argv) {

    Calendar *obj;
    argc != 2 ? printError(createCalendar("sample1.ics",&obj)) : printError(createCalendar(argv[1],&obj));

    printCalendar(obj);
    
    deleteCalendar(obj);
    return 0;
}

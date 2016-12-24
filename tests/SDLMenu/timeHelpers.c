#include "timeHelpers.h"

int timeBuffer[TIME_SLOT_HOUR+1] = {1,1,1,1,1,1};

void setupTimes() {
    
}
void readTime(int mode) {
    //UPDATE TIME
    //SET TO REAL VALUES
    /*
    timeInfo[TIME_DAY_OF_WEEK] = 1;
    timeInfo[TIME_DAY_OF_MONTH] = 2;
    timeInfo[TIME_MONTH] = 3;
    timeInfo[TIME_YEAR] = 4;
    timeInfo[TIME_SECOND] = 5;
    timeInfo[TIME_MINUTE] = 6;
    timeInfo[TIME_HOUR] = 7;
*/
}

void writeTime(int mode) {

}

char *slotToString(int slot) {
    switch(slot) {
        case TIME_SLOT_DAY_OF_WEEK: return "Day of Week";
        case TIME_SLOT_DAY_OF_MONTH: return "Day of Month";
        case TIME_SLOT_MONTH: return "Month";
        case TIME_SLOT_YEAR: return "Year";
        case TIME_SLOT_SECOND: return "Second";
        case TIME_SLOT_MINUTE: return "Minute";
        case TIME_SLOT_HOUR: return "Hour";
    }
    return "";
}


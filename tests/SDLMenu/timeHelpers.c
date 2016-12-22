#include "timeHelpers.h"

int timeBuffer[6] = {1,1,1,1,1,1};

TimeInfo timeInfos[TIME_MODE_ALARM][TIME_SLOT_HOUR];

void setupTimes() {
    int mode, slot;
    for(mode = 0; mode < TIME_MODE_ALARM; mode++) {
        for(slot = 0; slot < TIME_SLOT_HOUR; slot++) {
             timeInfos[mode][slot].slot = slot;
            timeInfos[mode][slot].mode = mode;
           
        }
    }
}
void readTime(TimeInfo *timeInfo) {
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

void writeTime(TimeInfo *timeInfo) {

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


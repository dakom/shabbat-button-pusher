#include "time.h"

int timeInfo[6];

void setToCurrentTime() {
    //UPDATE TIME
    //SET TO REAL VALUES
    timeInfo[TIME_DAY_OF_WEEK] = 1;
    timeInfo[TIME_DAY_OF_MONTH] = 2;
    timeInfo[TIME_MONTH] = 3;
    timeInfo[TIME_YEAR] = 4;
    timeInfo[TIME_SECOND] = 5;
    timeInfo[TIME_MINUTE] = 6;
    timeInfo[TIME_HOUR] = 7;
}

void setToAlarmTime() {
    
}
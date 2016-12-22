#ifndef TIMEHELPERS_H
#define TIMEHELPERS_H

#define TIME_SLOT_DAY_OF_WEEK 0
#define TIME_SLOT_DAY_OF_MONTH 1
#define TIME_SLOT_MONTH 2
#define TIME_SLOT_YEAR 3
#define TIME_SLOT_SECOND 4
#define TIME_SLOT_MINUTE 5
#define TIME_SLOT_HOUR 6

#define TIME_MODE_CLOCK 0
#define TIME_MODE_SHABBAT_TOGGLE 1
#define TIME_MODE_ALARM 2


typedef struct TimeInfo{
  int slot;
  int mode;
};

extern int timeBuffer[];
extern TimeInfo timeInfos[TIME_MODE_ALARM][TIME_SLOT_HOUR];

void setupTimes();
void readTime(TimeInfo *);
void writeTime(TimeInfo *);
char *slotToString(int);

#endif
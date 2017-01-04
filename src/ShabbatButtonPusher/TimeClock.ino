void setupTimes() {
  rtc.begin(PIN_CLOCK);
  rtc.autoTime();
  manualAlarm[TIME_SLOT_HOUR] = -1;

  shabbosAlarmActive = true;
}

/*
 * Original design only called updateTime() when needed for display
 * And used rtc.alarm1() and rtc.alarm2() - but since we might want to
 * Display the time always (e.g. clock mode), made sense just to make the alarms generic
 * For any type of rtc, even if alarm() functions weren't available.
 * This might benefit from optimization especially when battery powered...
 */
void updateTime() {
  rtc.update();

  timeBuffer[TIME_SLOT_DAY_OF_WEEK] = rtc.day();
  timeBuffer[TIME_SLOT_DAY_OF_MONTH] = rtc.date();
  timeBuffer[TIME_SLOT_MONTH] = rtc.month();
  timeBuffer[TIME_SLOT_YEAR] = rtc.year();
  timeBuffer[TIME_SLOT_SECOND] = rtc.second();
  timeBuffer[TIME_SLOT_MINUTE] = rtc.minute();
  timeBuffer[TIME_SLOT_HOUR] = rtc.hour();
  
  bool motorActive = false;

  if (manualAlarm[TIME_SLOT_HOUR] != -1) {
    if (manualAlarm[TIME_SLOT_DAY_OF_MONTH] == timeBuffer[TIME_SLOT_DAY_OF_MONTH]
        && manualAlarm[TIME_SLOT_MONTH] == timeBuffer[TIME_SLOT_MONTH]
        && manualAlarm[TIME_SLOT_YEAR] == timeBuffer[TIME_SLOT_YEAR]
        && manualAlarm[TIME_SLOT_SECOND] == timeBuffer[TIME_SLOT_SECOND]
        && manualAlarm[TIME_SLOT_MINUTE] == timeBuffer[TIME_SLOT_MINUTE]
        && manualAlarm[TIME_SLOT_HOUR] == timeBuffer[TIME_SLOT_HOUR]) {

      motorActive = true;
      manualAlarm[TIME_SLOT_HOUR] = -1;
    }
/*
    log("[%d %d] [%d %d] [%d %d] [%d %d] [%d %d] [%d %d]", manualAlarm[TIME_SLOT_DAY_OF_MONTH], timeBuffer[TIME_SLOT_DAY_OF_MONTH]
        ,manualAlarm[TIME_SLOT_MONTH], timeBuffer[TIME_SLOT_MONTH]
        , manualAlarm[TIME_SLOT_YEAR],timeBuffer[TIME_SLOT_YEAR]
        , manualAlarm[TIME_SLOT_SECOND], timeBuffer[TIME_SLOT_SECOND]
        , manualAlarm[TIME_SLOT_MINUTE] , timeBuffer[TIME_SLOT_MINUTE]
       , manualAlarm[TIME_SLOT_HOUR], timeBuffer[TIME_SLOT_HOUR]);
*/
    
  }

  if (!motorActive && shabbosAlarmActive) {
    if (timeBuffer[TIME_SLOT_DAY_OF_WEEK] == 7
        && timeBuffer[TIME_SLOT_HOUR] == 13
        && timeBuffer[TIME_SLOT_MINUTE] == 1
        && timeBuffer[TIME_SLOT_SECOND] == 1) {
      motorActive = true;
    }
  }
  if (motorActive) {
    changeMotorOn();
  }
}

void writeTime(int mode) {
  if (mode == TIME_MODE_CLOCK) {
    rtc.setTime(timeBuffer[TIME_SLOT_SECOND], timeBuffer[TIME_SLOT_MINUTE], timeBuffer[TIME_SLOT_HOUR], timeBuffer[TIME_SLOT_DAY_OF_WEEK], timeBuffer[TIME_SLOT_DAY_OF_MONTH], timeBuffer[TIME_SLOT_MONTH],  timeBuffer[TIME_SLOT_YEAR]);
  } else if (mode == TIME_MODE_ALARM) {
    //logString("SET ALARM!");
    //logString(GetCurrentTimeString());
    //logString(GetCurrentDateString());
    
    manualAlarm[TIME_SLOT_DAY_OF_WEEK] = timeBuffer[TIME_SLOT_DAY_OF_WEEK];
    manualAlarm[TIME_SLOT_DAY_OF_MONTH] = timeBuffer[TIME_SLOT_DAY_OF_MONTH];
    manualAlarm[TIME_SLOT_MONTH] = timeBuffer[TIME_SLOT_MONTH];
    manualAlarm[TIME_SLOT_YEAR] = timeBuffer[TIME_SLOT_YEAR];
    manualAlarm[TIME_SLOT_SECOND] = 1; //just do it at the start of the minute...
    manualAlarm[TIME_SLOT_MINUTE] = timeBuffer[TIME_SLOT_MINUTE];
    manualAlarm[TIME_SLOT_HOUR] = timeBuffer[TIME_SLOT_HOUR];
  }


}


String slotToString(int slot) {
  switch (slot) {
    case TIME_SLOT_DAY_OF_WEEK: return F("Day of Week");
    case TIME_SLOT_DAY_OF_MONTH: return F("Day of Month");
    case TIME_SLOT_MONTH: return F("Month");
    case TIME_SLOT_YEAR: return F("Year");
    case TIME_SLOT_SECOND: return F("Second");
    case TIME_SLOT_MINUTE: return F("Minute");
    case TIME_SLOT_HOUR: return F("Hour");
  }
  return "";
}

String GetCurrentDateString() {
  String temp;

  // Read the day/date:
  // temp.concat(timeInfo->dayStr);

  temp.concat(" ") ;
  temp.concat(timeBuffer[TIME_SLOT_DAY_OF_MONTH]);
  temp.concat("/");
  temp.concat(timeBuffer[TIME_SLOT_MONTH]);
  temp.concat("/") ;
  temp.concat(timeBuffer[TIME_SLOT_YEAR]);

  return (temp);
}

String GetCurrentTimeString() {
  String temp;
  // Read the time:
  temp.concat(timeBuffer[TIME_SLOT_HOUR]);
  temp.concat(":") ;
  temp.concat(timeBuffer[TIME_SLOT_MINUTE]);
  temp.concat(":") ;
  temp.concat(timeBuffer[TIME_SLOT_SECOND]);

  return (temp);
}

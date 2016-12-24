void setupTimes() {
    rtc.begin(PIN_CLOCK);
    rtc.autoTime();
}

void readTime(int mode) {
  rtc.update();

  if(mode == TIME_MODE_CLOCK) {
    timeBuffer[TIME_SLOT_DAY_OF_WEEK] = rtc.day();
    timeBuffer[TIME_SLOT_DAY_OF_MONTH] = rtc.date();
    timeBuffer[TIME_SLOT_MONTH] = rtc.month();
    timeBuffer[TIME_SLOT_YEAR] = rtc.year();
    timeBuffer[TIME_SLOT_SECOND] = rtc.second();
    timeBuffer[TIME_SLOT_MINUTE] = rtc.minute();
    timeBuffer[TIME_SLOT_HOUR] = rtc.hour();

    log("DAY OF WEEK: %d %s", rtc.day(), rtc.dayStr());
  } else {
    //TODO: read from alarm setting
  }
}

void writeTime(int mode) {
  if(mode == TIME_MODE_CLOCK) {
    rtc.setTime(timeBuffer[TIME_SLOT_SECOND], timeBuffer[TIME_SLOT_MINUTE], timeBuffer[TIME_SLOT_HOUR],timeBuffer[TIME_SLOT_DAY_OF_WEEK], timeBuffer[TIME_SLOT_DAY_OF_MONTH], timeBuffer[TIME_SLOT_MONTH],  timeBuffer[TIME_SLOT_YEAR]);
  } else {
    //TODO: set alarm setting
  }

  //NOTE DAY OF WEEK = 7 WHEN IT'S SHABBOS DAY (SATURDAY)
}


String slotToString(int slot) {
    switch(slot) {
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

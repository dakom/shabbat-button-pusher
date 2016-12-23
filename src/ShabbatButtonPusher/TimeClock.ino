void setupTimes() {
    rtc.begin(PIN_CLOCK);
    rtc.autoTime();
  
    int mode, slot;
    for(mode = 0; mode < TIME_MODE_ALARM; mode++) {
        for(slot = 0; slot < TIME_SLOT_HOUR; slot++) {
             timeInfos[mode][slot].slot = slot;
            timeInfos[mode][slot].mode = mode;
           
        }
    }
}

void readTime(TimeInfo *timeInfo) {
  rtc.update();

  if(timeInfo->mode == TIME_MODE_CLOCK) {
    timeBuffer[TIME_SLOT_DAY_OF_WEEK] = rtc.day();
    timeBuffer[TIME_SLOT_DAY_OF_MONTH] = rtc.date();
    timeBuffer[TIME_SLOT_MONTH] = rtc.month();
    timeBuffer[TIME_SLOT_YEAR] = rtc.year();
    timeBuffer[TIME_SLOT_SECOND] = rtc.second();
    timeBuffer[TIME_SLOT_MINUTE] = rtc.minute();
    timeBuffer[TIME_SLOT_HOUR] = rtc.hour();
  } else {
    //TODO: read from alarm setting
  }
}

void writeTime(TimeInfo *timeInfo) {
  //TODO: Write time!
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

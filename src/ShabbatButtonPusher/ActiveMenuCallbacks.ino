void changeMotorOn() {
  motors.state = MOTOR_STATE_START;
  showText("Pushing Button...");
}

void changeShabbosOn() {
  showText("Changed Shabbos On!");
   //TODO: SET ALARM
  delay(MENU_DELAY);
  chooseMenu(&menus[0]);
}

void changeShabbosOff() {
  showText("Changed Shabbos Off!");
  //TODO: Delete Alarm
  delay(MENU_DELAY);
  chooseMenu(&menus[0]);
}

void displayTimeChildDisplay()
{
  readTime(TIME_MODE_CLOCK);
  showFullDisplay(GetCurrentDateString(), GetCurrentTimeString());
  delay(MENU_DELAY);
  chooseMenu(&menus[0]);
}

void displayTimeSlot() {

  //instead of showText because slotToString returns a String
  showFullDisplay(slotToString(visibleMenu->timeInfo->slot), F(""));
}

int getTimeStart() {

  readTime(visibleMenu->timeInfo);
  return timeBuffer[visibleMenu->timeInfo->slot];
}

int getTimeMax() {
  switch(visibleMenu->timeInfo->slot) {
    case TIME_SLOT_DAY_OF_WEEK: return 7;
    case TIME_SLOT_DAY_OF_MONTH: return 31;
    case TIME_SLOT_MONTH: return 12;
    case TIME_SLOT_YEAR: return 10000;
    case TIME_SLOT_HOUR: return 24;
    case TIME_SLOT_MINUTE: return 60;
    case TIME_SLOT_SECOND: return 60;
  }
}

void displaySetTime() {
  String val = GetFormattedString("%d", verticalIndex);
  showFullDisplay(slotToString(visibleMenu->timeInfo->slot), val);
  
}

void changeTime() {
  timeBuffer[visibleMenu->timeInfo->slot] = verticalIndex;
  writeTime(visibleMenu->timeInfo);
  showFullDisplay(slotToString(visibleMenu->timeInfo->slot), F("Updated!"));
 
  delay(MENU_DELAY);
  chooseMenu(&menus[0]);
}

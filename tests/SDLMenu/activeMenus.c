#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "timeHelpers.h"
#include <SDL.h>

void genericCancelDisplay()
{
  showText("CANCEL");
}

void genericCancelProcessButton()
{
  chooseMenu(&menus[0]);
}

void genericAcceptChildProcessButton()
{
  chooseMenu(getSelectedChild());
}

void genericUpdateVisibleDisplay() {
  visibleMenu->showDisplay();
}

void mainMenuShowDisplay()
{
  showText("Good Shabbos!");
}

void mainMenuProcessButton()
{
  if (currentButton != BTN_NONE)
  {
      chooseMenu(getSelectedChild());
  }
}

void mainChildStartMotorDisplay()
{
  showText("Start Motor Now");
}


void mainChildDisplayTimeDisplay()
{
  showText("Display Time");
}

void mainChildSetTimeDisplay()
{
  showText("Set Time");
}

void mainChildSetAlarmDisplay()
{
  showText("Set Alarm");
}

void mainChildToggleShabbosDisplay() {
  showText("Toggle Shabbos Mode");
}

void toggleShabbosOnDisplay() {
  showText("Turn on Shabbos (1:00PM)");
}

void toggleShabbosOffDisplay() {
  showText("Turn off Shabbos");
}

void changeShabbosOn() {
  showText("Changed Shabbos On!");
  SDL_Delay(1000);
  chooseMenu(&menus[0]);
}

void changeShabbosOff() {
  showText("Changed Shabbos Off!");
  SDL_Delay(1000);
  chooseMenu(&menus[0]);
}

void displayTimeChildDisplay()
{
  showText("Current Time!");
  SDL_Delay(1000);
  chooseMenu(&menus[0]);
}

void displayTimeSlot() {
  
  showText(slotToString(visibleMenu->timeInfo->slot));
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
  showText("%s: %d", slotToString(visibleMenu->timeInfo->slot), verticalIndex);
}

void changeTime() {
  timeBuffer[visibleMenu->timeInfo->slot] = verticalIndex;
  writeTime(visibleMenu->timeInfo);
  showText("Changed %s!", slotToString(visibleMenu->timeInfo->slot));
  SDL_Delay(1000);
  chooseMenu(&menus[0]);
}


#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "time.h"
#include <SDL.h>

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




void displayTimeChildDisplay()
{
  showText("Current Time!");
}

void displayDayOfWeek()
{
  showText("Day of Week");
}

void displayDayOfMonth()
{
  showText("Day of Month");
}

void displayYear()
{
  showText("Year");
}

void displayMonthOfYear()
{
  showText("Month");
}

void displayHour()
{
  showText("Hour");
}

void displayMinute()
{
  showText("Minute");
}

void displaySecond()
{
  showText("Second");
}


int getDayOfWeekStart()
{
  setToCurrentTime();
  return(timeInfo[TIME_DAY_OF_WEEK]);
}

int getDayOfMonthStart()
{
  setToCurrentTime();
  return(timeInfo[TIME_DAY_OF_MONTH]);
}

int getMonthOfYearStart()
{
  setToCurrentTime();
  return(timeInfo[TIME_MONTH]);
}

int getYearStart()
{
  setToCurrentTime();
  return(timeInfo[TIME_YEAR]);
}

int getHourStart()
{
  setToCurrentTime();
  return(timeInfo[TIME_HOUR]);
}

int getMinuteStart()
{
  setToCurrentTime();
  return(timeInfo[TIME_MINUTE]);
}

int getSecondStart()
{
  setToCurrentTime();
  return(timeInfo[TIME_SECOND]);
}



int getDayOfMonthMax()
{
  return(31); //should be determined by current date
}

int getMonthOfYearMax()
{
  return(12);
}

int getYearMax()
{
  return(10000);
}

int getHourMax()
{
  return(24);
}

int getMinuteMax()
{
  return(60);
}

int getSecondMax()
{
  return(60);
}


void displaySetDayOfWeek()
{
  showText("Day of Week: %d", verticalIndex);
}

void displaySetDayOfMonth()
{
  showText("Day of Month: %d", verticalIndex);
}

void displaySetMonthOfYear()
{
  showText("Month: %d", verticalIndex);
}

void displaySetYear()
{
  showText("Year: %d", verticalIndex);
}

void displaySetHour()
{
  showText("Hour: %d", verticalIndex);
}

void displaySetMinute()
{
  showText("Minute: %d", verticalIndex);
}

void displaySetSecond()
{
  showText("Second: %d", verticalIndex);
}



void changeCurrentDayOfWeek()
{
  timeInfo[TIME_DAY_OF_WEEK] = verticalIndex;
  showText("Changed Day of Week!");
}

void changeCurrentDayOfMonth()
{
  timeInfo[TIME_DAY_OF_MONTH] = verticalIndex;
  showText("Changed Day of Month: %d", verticalIndex);
}

void changeCurrentMonthOfYear()
{
  timeInfo[TIME_MONTH] = verticalIndex;
  showText("Changed Month: %d", verticalIndex);
}

void changeCurrentYear()
{
  timeInfo[TIME_YEAR] = verticalIndex;
  showText("Changed Year: %d", verticalIndex);
}

void changeCurrentHour()
{
  timeInfo[TIME_HOUR] = verticalIndex;
  showText("Hour: %d", verticalIndex);
}

void changeCurrentMinute()
{
  timeInfo[TIME_MINUTE] = verticalIndex;
  showText("Changed Minute!");
  SDL_Delay(1000);
  chooseMenu(&menus[0]);
}

void changeCurrentSecond()
{
  timeInfo[TIME_SECOND] = verticalIndex;
  showText("Changed Second!");
  SDL_Delay(1000);
  chooseMenu(&menus[0]);
}

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
  MenuInfo *visibleMenu = getVisibleMenu();
  visibleMenu->showDisplay();
}
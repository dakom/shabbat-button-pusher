#include <stdlib.h>
#include <stdio.h>
#include "menu.h"

void mainMenuShowDisplay()
{
  showText("Good Shabbos!");
}

void mainMenuProcessButton()
{
  if (currentButton != BTN_NONE)
  {
      assignMenu(getSelectedChild());
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

void genericCancelDisplay()
{
  showText("CANCEL");
}

void genericCancelProcessButton()
{
  if (currentButton == BTN_ENTER)
  {
      assignMenu(&menus[0]);
  }
}

void genericAcceptChildProcessButton()
{
  if (currentButton == BTN_ENTER)
  {
      assignMenu(getSelectedChild());
  }
}
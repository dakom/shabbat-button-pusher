#include <stdio.h>
#include "menu.h"
#include "activeMenus.h"
#include "setupMenus.h"


/*
 * This is in its own file so that auto-formatting can be disabled
 * Easier to see the hierarchy this way :)
 */

void setupMenus()
{
  int index = 0;
  MenuInfo *level[5]; //hardcode this to maximum depth
  
  ROOT
    PROCESS(0, &mainMenuShowDisplay, &mainMenuProcessButton)
      DISPLAY(1, &mainChildStartMotorDisplay)
      ACCEPT(1, &mainChildDisplayTimeDisplay)
        DISPLAY(2, &displayTimeChildDisplay)
      
      ACCEPT(1, &mainChildSetTimeDisplay)
        TIME_MENU(2, TIME_MODE_CLOCK)
        CANCEL(2)

      ACCEPT(1, &mainChildSetAlarmDisplay)
        TIME_MENU(2, TIME_MODE_ALARM)
        CANCEL(2)
      
      ACCEPT(1, &mainChildToggleShabbosDisplay)
        PROCESS(2, &toggleShabbosOnDisplay, &changeShabbosOn)
        PROCESS(2,  &toggleShabbosOffDisplay, &changeShabbosOff)
        CANCEL(2)
      CANCEL(1)
  
  START_ROOT()

  printf("%d menus created, %d wasted in NUMBER_OF_MENUS static allocation :P\n", index, NUMBER_OF_MENUS - index);
}
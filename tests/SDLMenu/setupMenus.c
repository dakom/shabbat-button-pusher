#include <stdio.h>
#include "menu.h"
#include "activeMenus.h"

/*
 * This is in its own file so that auto-formatting can be disabled
 * Easier to see the hierarchy this way :)
 */

void setupMenus()
{
  int index = 0;
  MenuInfo *level[5];
  
  level[0] = createMenu(NULL, index++, NULL, NULL, NULL, NULL);
    level[1] = createMenu(level[0], index++, &mainMenuShowDisplay, &mainMenuProcessButton, NULL,  NULL);
      level[2] = createMenu(level[1], index++, &mainChildStartMotorDisplay, NULL, NULL,  NULL);
      level[2] = createMenu(level[1], index++, &mainChildDisplayTimeDisplay, &genericAcceptChildProcessButton, NULL,  NULL);
        level[3] = createMenu(level[2], index++, &displayTimeChildDisplay, NULL, NULL,  NULL);
        level[3] = createMenu(level[2], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);
      
      level[2] = createMenu(level[1], index++, &mainChildSetTimeDisplay,  &genericAcceptChildProcessButton, NULL,  NULL);

        level[3] = createMenu(level[2], index++, &displayDayOfWeek, &genericAcceptChildProcessButton, NULL,  NULL);
          level[4] = createMenu(level[3], index++, &displaySetDayOfWeek, NULL, &getDayOfWeekStart,  &getDayOfWeekMax);
          level[4] = createMenu(level[3], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);
       
        level[3] = createMenu(level[2], index++, &displayDayOfMonth, &genericAcceptChildProcessButton, NULL,  NULL);
          level[4] = createMenu(level[3], index++, &displaySetDayOfMonth, NULL, &getDayOfMonthStart,  &getDayOfMonthMax); 
          level[4] = createMenu(level[3], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);
        
        level[3] = createMenu(level[2], index++, &displayMonthOfYear, &genericAcceptChildProcessButton, NULL,  NULL);
          level[4] = createMenu(level[3], index++, &displaySetMonthOfYear, NULL, &getMonthOfYearStart, &getMonthOfYearMax);
          level[4] = createMenu(level[3], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);

        level[3] = createMenu(level[2], index++, &displayYear, &genericAcceptChildProcessButton, NULL,  NULL);
          level[4] = createMenu(level[3], index++, &displaySetYear, NULL, &getYearStart, &getYearMax);
          level[4] = createMenu(level[3], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);
        
        level[3] = createMenu(level[2], index++, &displayHour, &genericAcceptChildProcessButton, NULL,  NULL);
          level[4] = createMenu(level[3], index++, &displaySetHour, NULL, &getHourStart,  &getHourMax);
          level[4] = createMenu(level[3], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);
        
        level[3] = createMenu(level[2], index++, &displayMinute, &genericAcceptChildProcessButton, NULL,  NULL);
          level[4] = createMenu(level[3], index++, &displaySetMinute, NULL, &getMinuteStart,  &getMinuteMax);
          level[4] = createMenu(level[3], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);
        
        level[3] = createMenu(level[2], index++, &displaySecond, &genericAcceptChildProcessButton, NULL,  NULL);
          level[4] = createMenu(level[3], index++, &displaySetSecond, NULL, &getSecondStart,  &getSecondMax);
          level[4] = createMenu(level[3], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);
        
        level[3] = createMenu(level[2], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);
        
      level[2] = createMenu(level[1], index++, &mainChildSetAlarmDisplay, NULL, NULL,  NULL);
      level[2] = createMenu(level[1], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL);

  chooseMenu(&menus[0]);

  printf("%d menus created, %d wasted in NUMBER_OF_MENUS static allocation :P\n", index, NUMBER_OF_MENUS - index);
}
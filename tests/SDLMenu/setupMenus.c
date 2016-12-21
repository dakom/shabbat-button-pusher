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
  MenuInfo *menu;
  MenuInfo *parent;
  
  parent = createMenu(NULL, index++, NULL, NULL, NULL);
    parent = createMenu(parent, index++, &mainMenuShowDisplay, &mainMenuProcessButton, NULL);
      createMenu(parent, index++, &mainChildStartMotorDisplay, NULL, NULL);
      menu = createMenu(parent, index++, &mainChildDisplayTimeDisplay, &genericAcceptChildProcessButton, NULL);
        createMenu(menu, index++, &displayTimeChildDisplay, NULL, NULL);
        createMenu(menu, index++, &genericCancelDisplay, &genericCancelProcessButton, NULL);

      createMenu(parent, index++, &mainChildSetTimeDisplay, NULL, NULL);
      createMenu(parent, index++, &mainChildSetAlarmDisplay, NULL, NULL);
      createMenu(parent, index++, &genericCancelDisplay, &genericCancelProcessButton, NULL);

  chooseMenu(&menus[0]);

  printf("%d menus created, %d wasted in NUMBER_OF_MENUS static allocation :P\n", index, NUMBER_OF_MENUS - index);
}
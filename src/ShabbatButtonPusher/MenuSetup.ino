/*
 * I guess this is what happens when you're coding C, used to C#, and don't know C++ :) 
 */
 
#define ROOT\
  level[0] = createMenu(NULL, index++, NULL, NULL, NULL, NULL, -1, -1);

#define PROCESS(l, d, p)\
  level[l+1] = createMenu(level[l], index++, d, p, NULL,  NULL,  -1, -1);

#define DISPLAY(l, d)\
  PROCESS(l, d, NULL)
  
#define ACCEPT(l, d)\
  PROCESS(l, d, &genericAcceptChildProcessButton)

#define CANCEL(l)\
  PROCESS(l,  &genericCancelDisplay, &genericCancelProcessButton)

#define TIME_SUBMENU(l, m, s)\
  level[l+1] = createMenu(level[l], index++, &displayTimeSlot, &genericAcceptChildProcessButton, NULL,  NULL, m, s);\
  level[l+2] = createMenu(level[l+1], index++, &displaySetTime, &changeTime, &getTimeStart,  &getTimeMax, m, s);\
  CANCEL(l+1)
  
#define TIME_MENU(l,m)\
  TIME_SUBMENU(l, m, TIME_SLOT_DAY_OF_WEEK)\
  TIME_SUBMENU(l, m, TIME_SLOT_DAY_OF_MONTH)\
  TIME_SUBMENU(l, m, TIME_SLOT_MONTH)\
  TIME_SUBMENU(l, m, TIME_SLOT_YEAR)\
  TIME_SUBMENU(l, m, TIME_SLOT_HOUR)\
  TIME_SUBMENU(l, m, TIME_SLOT_MINUTE)\
  TIME_SUBMENU(l, m, TIME_SLOT_SECOND)\

#define START_ROOT()\
  chooseMenu(&menus[0]);\
  visibleMenu->showDisplay();

MenuInfo *createMenu(MenuInfo *parent, int index, void (*showDisplay)(), void (*onAccept)(), int (*getVerticalStart)(), int (*getVerticalMax)(), int timeMode, int timeSlot)
{

  MenuInfo *menu = &menus[index];

  menu->nextSibling = menu->parent = menu->firstChild = NULL;
  menu->showDisplay = showDisplay;
  menu->onAccept = onAccept;
  menu->getVerticalStart = getVerticalStart;
  menu->getVerticalMax = getVerticalMax;
  menu->maxChildIndex = -1;
  menu->timeMode = timeMode;
  menu->timeSlot = timeSlot;

  if (parent != NULL)
  {
    menu->parent = parent;

    if (parent->firstChild == NULL)
    {
      parent->firstChild = menu;
    }
    else
    {
      MenuInfo *sibling = parent->firstChild;
      while (sibling->nextSibling != NULL)
      {
        sibling = sibling->nextSibling;
      }
      sibling->nextSibling = menu;
    }
  }

  return menu;
}

/*
 * Called in main setup
 */
 
void setupMenus()
{
  int index = 0;
  MenuInfo *level[5]; //if expanding menu deeper, adjust this
  
  ROOT
    PROCESS(0, &mainMenuShowDisplay, &genericSelectThroughAnyButton)
      ACCEPT(1, &mainChildStartMotorDisplay)
        DISPLAY(2, &changeMotorOn)
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

  //log("%d menus created, %d wasted in NUMBER_OF_MENUS static allocation :P\n", index, NUMBER_OF_MENUS - index);
}


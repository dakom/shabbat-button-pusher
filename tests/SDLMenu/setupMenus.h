#define ROOT\
  level[0] = createMenu(NULL, index++, NULL, NULL, NULL, NULL, NULL);

#define PROCESS(l, d, p)\
  level[l+1] = createMenu(level[l], index++, d, p, NULL,  NULL, NULL);

#define DISPLAY(l, d)\
  PROCESS(l, d, NULL)
  
#define ACCEPT(l, d)\
  PROCESS(l, d, &genericAcceptChildProcessButton)

#define CANCEL(l)\
  PROCESS(l,  &genericCancelDisplay, &genericCancelProcessButton)

#define TIME_SUBMENU(l, m, s)\
  level[l+1] = createMenu(level[l], index++, &displayTimeSlot, &genericAcceptChildProcessButton, NULL,  NULL, &timeInfos[m][s]);\
  level[l+2] = createMenu(level[l+1], index++, &displaySetTime, &changeTime, &getTimeStart,  &getTimeMax, &timeInfos[m][s]);\
  level[l+2] = createMenu(level[l+1], index++, &genericCancelDisplay, &genericCancelProcessButton, NULL,  NULL, NULL);

#define TIME_MENU(l,m)\
  TIME_SUBMENU(l, m, TIME_SLOT_DAY_OF_WEEK)\
  TIME_SUBMENU(l, m, TIME_SLOT_DAY_OF_MONTH)\
  TIME_SUBMENU(l, m, TIME_SLOT_MONTH)\
  TIME_SUBMENU(l, m, TIME_SLOT_YEAR)\
  TIME_SUBMENU(l, m, TIME_SLOT_HOUR)\
  TIME_SUBMENU(l, m, TIME_SLOT_MINUTE)\
  TIME_SUBMENU(l, m, TIME_SLOT_SECOND)\

#define START_ROOT() chooseMenu(&menus[0]); 
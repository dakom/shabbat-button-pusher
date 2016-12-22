#include <LiquidCrystal.h>
#include <SPI.h>
#include <SparkFunDS3234RTC.h>
#include <Servo.h>


///////////////////////////////////////////////////////////////////
//////////////// DEFINES //////////////////////////////////////////
///////////////////////////////////////////////////////////////////

/*
 * PINS
 */
#define PIN_MOTOR 3
#define PIN_CLOCK 10

#define PIN_LCD_1 8
#define PIN_LCD_2 9
#define PIN_LCD_3 4
#define PIN_LCD_4 5
#define PIN_LCD_5 6
#define PIN_LCD_6 7

#define PIN_DISPLAY_BUTTON A0

/*
 * MOTORS
 */
#define MOTOR_STATE_NEUTRAL 0
#define MOTOR_STATE_START 1
#define MOTOR_STATE_FORWARD 2
#define MOTOR_STATE_REVERSE 3
#define MOTOR_POSITION_ORIGIN 30
#define MOTOR_POSITION_END 170

typedef struct MotorInfo {
  Servo servo;
  uint8_t state;
};

/*
 * INPUT
 */
#define BTN_NONE 0
#define BTN_LEFT 1
#define BTN_RIGHT 2
#define BTN_UP 3
#define BTN_DOWN 4
#define BTN_ENTER 5

/*
 * TIME
 */
#define TIME_SLOT_DAY_OF_WEEK 0
#define TIME_SLOT_DAY_OF_MONTH 1
#define TIME_SLOT_MONTH 2
#define TIME_SLOT_YEAR 3
#define TIME_SLOT_SECOND 4
#define TIME_SLOT_MINUTE 5
#define TIME_SLOT_HOUR 6

#define TIME_MODE_CLOCK 0
#define TIME_MODE_SHABBAT_TOGGLE 1
#define TIME_MODE_ALARM 2


typedef struct TimeInfo {
  int slot;
  int mode;
};

/*
 * MENU
 */

#define showText(s) showTextStatic(F(s))

#define NUMBER_OF_MENUS 56 // change this number to real amount for optimization

#define MENU_DELAY 3000

typedef struct MenuInfo{
  void (*showDisplay)();
  void (*onAccept)();
  int (*getVerticalStart)();
  int (*getVerticalMax)(); 
  int maxChildIndex;
  TimeInfo *timeInfo;
  struct MenuInfo *nextSibling, *parent, *firstChild;
};



///////////////////////////////////////////////////////////////////
//////////////// GLOBALS //////////////////////////////////////////
///////////////////////////////////////////////////////////////////
/*
 * Menu
 */
MenuInfo menus[NUMBER_OF_MENUS];
int currentButton = BTN_NONE - 1; //impossible value
MenuInfo *currentMenu = NULL;
MenuInfo *visibleMenu = NULL;
int horizontalIndex = 0;
int verticalIndex = 0;

/*
 * Display
 */
LiquidCrystal lcd(PIN_LCD_1, PIN_LCD_2, PIN_LCD_3, PIN_LCD_4, PIN_LCD_5, PIN_LCD_6);

/*
 * Time
 */
int timeBuffer[6];
TimeInfo timeInfos[TIME_MODE_ALARM][TIME_SLOT_HOUR];

/*
 * Motors
 */

MotorInfo motors;

///////////////////////////////////////////////////////
//////////////// UTILS ////////////////////////////
///////////////////////////////////////////////////////

void log(char *fmt, ... ) {
  
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  
  Serial.print(buf);
  Serial.print('\n');
}



///////////////////////////////////////////////////////
//////////////// TIMECLOCK ////////////////////////////
///////////////////////////////////////////////////////

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

char *slotToString(int slot) {
    switch(slot) {
        case TIME_SLOT_DAY_OF_WEEK: return "Day of Week";
        case TIME_SLOT_DAY_OF_MONTH: return "Day of Month";
        case TIME_SLOT_MONTH: return "Month";
        case TIME_SLOT_YEAR: return "Year";
        case TIME_SLOT_SECOND: return "Second";
        case TIME_SLOT_MINUTE: return "Minute";
        case TIME_SLOT_HOUR: return "Hour";
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


///////////////////////////////////////////////////////
//////////////// MOTORS ///////////////////////////////
///////////////////////////////////////////////////////

void updateMotorState() {

  switch (motors.state) {
    //the hardcoded pos assignments should always be there inherently, it's just a sanity enforcement
    case MOTOR_STATE_FORWARD:
      motors.servo.write(MOTOR_POSITION_END);
      motors.state = MOTOR_STATE_REVERSE;
      break;

    case MOTOR_STATE_REVERSE:
      motors.servo.write(MOTOR_POSITION_ORIGIN);
      motors.state = MOTOR_STATE_NEUTRAL;
       chooseMenu(&menus[0]);
      //loop forever - for testing
      //motors.state = MOTOR_STATE_START;
      break;
    case MOTOR_STATE_START:
      motors.servo.write(MOTOR_POSITION_ORIGIN);
      motors.state = MOTOR_STATE_FORWARD;
      break;
    default:
      //always return early if motor wasn't already running here...
      return;
  }

  //give the servo time to settle into position
  delay(2000);
}

void setupMotors() {
  pinMode(PIN_MOTOR, OUTPUT);

  motors.state = MOTOR_STATE_NEUTRAL;
  motors.servo.attach(PIN_MOTOR); //controlling several in parallel off the same pin
  motors.servo.write(MOTOR_POSITION_ORIGIN);
}

///////////////////////////////////////////////////////
//////////////// MENU SYSTEM //////////////////////////
///////////////////////////////////////////////////////

/*
 * Display
 */


void setupDisplay() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void showDisplay(String textToDisplayTop, String textToDisplayBottom) {
  lcd.clear();
  if (textToDisplayTop != "") {
    lcd.setCursor(0, 0);
    lcd.print(textToDisplayTop);
  }
  if (textToDisplayBottom != "") {
    lcd.setCursor(0, 1);
    lcd.print(textToDisplayBottom);
  }
}

void showTextStatic(String s) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(s);
}

void showTextFormat(char *fmt, ... ) {
  
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);

  showDisplay(buf, "");
}

/*
 * Input
 */
int GetKey(int value)
{

  int btn = BTN_NONE;

  if (value > 600 && value < 1000) {
    if (value < 650) {
      btn = BTN_ENTER;
    } else if (value < 840) {
      btn = BTN_RIGHT;
    } else if (value < 890) {
      btn = BTN_LEFT;
    } else if (value < 920) {
      btn = BTN_DOWN;
    } else if (value < 950) {
      btn = BTN_UP;
    }
  }

  return btn;
}

MenuInfo *getSelectedChild()
{
  MenuInfo *parent = currentMenu;

  if (parent->firstChild == NULL)
  {
    return NULL;
  }

  MenuInfo *child = parent->firstChild;
  for (int i = 0; i < horizontalIndex && child->nextSibling != NULL; i++)
  {
    child = child->nextSibling;
  }
  return child;
}

int getMaxChildIndex(MenuInfo *parent)
{
  int i = 0;
  if (parent->firstChild != NULL)
  {
    MenuInfo *child = parent->firstChild;
    for (i = 0; child->nextSibling != NULL; i++)
    {
      child = child->nextSibling;
    }
  }

  return i;
}

void updateButtonPress()
{
  int btn = GetKey(analogRead(PIN_DISPLAY_BUTTON));
 
  if (currentButton != btn)
  {
    currentButton = btn;
    /*
       * Yeah, it's kinda ugly, but it works :)
       * We are really always showing and processing the active child, not really the active menu
       * If there is no child then we show the current as a fallback
       * Since child is always determined by horizontalIndex, we can do sanity checks globally here
       * verticalIndex upper limit should be handled by each menu itself (bottom limit is always 0)
       */

    

    switch (currentButton)
    {
    case BTN_UP:
      verticalIndex++;
      break;
    case BTN_LEFT:
      horizontalIndex--;
      break;
    case BTN_RIGHT:
      horizontalIndex++;
      break;
    case BTN_DOWN:
      verticalIndex--;
      break;
    }

    if (currentMenu->maxChildIndex == -1)
    {
      //once per menu, delayed till we need it here
      currentMenu->maxChildIndex = getMaxChildIndex(currentMenu);
    }

    if (horizontalIndex < 0)
    {
      horizontalIndex = 0;
    }
    if (horizontalIndex > currentMenu->maxChildIndex) {
        horizontalIndex = currentMenu->maxChildIndex;
    }

    setVisibleMenu();

    int verticalMax = 0;

    if(visibleMenu->getVerticalMax != NULL) {
      verticalMax = visibleMenu->getVerticalMax();  
    }

    if (verticalIndex < 1)
    {
      verticalIndex = verticalMax;
    }

    if(verticalIndex > verticalMax) {
      verticalIndex = 1;
    }
    
    if (currentButton == BTN_ENTER && visibleMenu->onAccept != NULL) {
      visibleMenu->onAccept();
    }

    //they must always show something, however
    visibleMenu->showDisplay();
  }
}



void setVisibleMenu() {
  visibleMenu = getSelectedChild();

    if (visibleMenu == NULL)
    {
      visibleMenu = currentMenu;
    }

}


void chooseMenu(MenuInfo *menu)
{
  currentMenu = menu;
  horizontalIndex = 0;

  verticalIndex = 1;

  setVisibleMenu();

  if(visibleMenu != NULL) {
    if(visibleMenu->getVerticalStart != NULL) {
      verticalIndex = visibleMenu->getVerticalStart();
    }
  }
  
}



/*
 * MENU CALLBACKS
 */

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

void changeMotorOn() {
  motors.state = MOTOR_STATE_START;
  showText("Pushing Button...");
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
  showDisplay(GetCurrentDateString(), GetCurrentTimeString());
  delay(MENU_DELAY);
  chooseMenu(&menus[0]);
}

void displayTimeSlot() {
  
  showTextFormat(slotToString(visibleMenu->timeInfo->slot));
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
  showTextFormat("%s: %d", slotToString(visibleMenu->timeInfo->slot), verticalIndex);
}

void changeTime() {
  timeBuffer[visibleMenu->timeInfo->slot] = verticalIndex;
  writeTime(visibleMenu->timeInfo);
  showTextFormat("Changed %s!", slotToString(visibleMenu->timeInfo->slot));
  delay(MENU_DELAY);
  chooseMenu(&menus[0]);
}

/*
 * MENU CREATION
 * 
  */

MenuInfo *createMenu(MenuInfo *parent, int index, void (*showDisplay)(), void (*onAccept)(), int (*getVerticalStart)(), int (*getVerticalMax)(), TimeInfo *timeInfo)
{

  MenuInfo *menu = &menus[index];

  menu->nextSibling = menu->parent = menu->firstChild = NULL;
  menu->showDisplay = showDisplay;
  menu->onAccept = onAccept;
  menu->getVerticalStart = getVerticalStart;
  menu->getVerticalMax = getVerticalMax;
  menu->maxChildIndex = -1;
  menu->timeInfo = timeInfo;

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

////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);

  setupDisplay();
  setupTimes();
  setupMenus();
  setupMotors();
}

void loop()
{
  updateButtonPress();
  updateMotorState();
}

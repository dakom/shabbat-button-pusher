#include <LiquidCrystal.h>
#include <SPI.h>
#include <SparkFunDS3234RTC.h>
#include <Servo.h>


///////////////////////////////////////////////////////////////////
//////////////// DEFINES AND TYPEDEFS /////////////////////////////
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
 * DISPLAY
 */

#define DISPLAY_TOP 1
#define DISPLAY_BOTTOM 2
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

/*
 * MENU
 */

#define showText(s) showFullDisplay(F(s), F(""))

#define NUMBER_OF_MENUS 60 // change this number to real amount for optimization

#define MENU_DELAY 3000

typedef struct MenuInfo{
  void (*showDisplay)();
  void (*onAccept)();
  int (*getVerticalStart)();
  int (*getVerticalMax)(); 
  int maxChildIndex;
  int timeMode;
  int timeSlot;
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
int timeBuffer[TIME_SLOT_HOUR+1];

/*
 * Motors
 */

MotorInfo motors;

///////////////////////////////////////////////////////////////////
//////////////// MAIN /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

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
  updateMenu();
  updateMotorState();
}

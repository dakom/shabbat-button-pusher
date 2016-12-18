#include <LiquidCrystal.h>
#include <SPI.h>
#include <SparkFunDS3234RTC.h>
#include <Servo.h>
/* GLOBALS

*/



#define PIN_MOTOR 3
#define MOTOR_STATE_NEUTRAL 0
#define MOTOR_STATE_START 1
#define MOTOR_STATE_FORWARD 2
#define MOTOR_STATE_REVERSE 3
#define MOTOR_POSITION_ORIGIN 30
#define MOTOR_POSITION_END 170

#define PIN_CLOCK 10

#define PIN_LCD_1 8
#define PIN_LCD_2 9
#define PIN_LCD_3 4
#define PIN_LCD_4 5
#define PIN_LCD_5 6
#define PIN_LCD_6 7

#define PIN_DISPLAY_BUTTON A0

#define MENU_NONE 0
#define MENU_MAIN_WAIT 1
#define MENU_MAIN_PROCESS 2
#define MENU_DISPLAY_TIME 3

#define BTN_NONE 0
#define BTN_LEFT 1
#define BTN_RIGHT 2
#define BTN_UP 3
#define BTN_DOWN 4
#define BTN_ENTER 5


LiquidCrystal lcd(PIN_LCD_1, PIN_LCD_2, PIN_LCD_3, PIN_LCD_4, PIN_LCD_5, PIN_LCD_6);

typedef struct {
  Servo servo;
  uint8_t state;
} MotorInfo;

MotorInfo motors;

typedef struct {
  int type;
  void (*process)();
  int choice;
  int button;
} MenuInfo;

MenuInfo menu;


/* UTILITIES

*/

void debugPrint(char *fmt, ... ) {
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.print(buf);
}

/*
   MOTORS
*/


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
      assignMenu(MENU_MAIN_WAIT);
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

/*
   CLOCK
*/



void setupClock() {
  rtc.begin(PIN_CLOCK);
  rtc.autoTime();
}

//=====================================
String ReadDate() {
  String temp;

  rtc.update(); // Update RTC data


  // Read the day/date:
  int dy = rtc.day();
  int da = rtc.date();
  int mo = rtc.month();
  int yr = rtc.year();

  String dayString = rtc.dayStr();

  temp.concat(dayString);

  temp.concat(" ") ;
  temp.concat(da);
  temp.concat("/");
  temp.concat(mo);
  temp.concat("/") ;
  temp.concat(yr);


  return (temp);
}

String ReadTime() {
  String temp;

  rtc.update(); // Update RTC data

  // Read the time:
  int s = rtc.second();
  int m = rtc.minute();
  int h = rtc.hour();
  temp.concat(h);
  temp.concat(":") ;
  temp.concat(m);
  temp.concat(":") ;
  temp.concat(s);

  return (temp);
}

/*
   MENU
*/

unsigned char GetKey(int value)
{
  if (value >= 850 && value <= 860) {
    return BTN_LEFT;
  } else if (value >= 810 && value <= 820) {
    return BTN_RIGHT;
  } else if (value >= 925 && value <= 935) {
    return BTN_UP;
  } else if (value >= 900 && value <= 910) {
    return BTN_DOWN;
  } else if (value >= 610 && value <= 620) {
    return BTN_ENTER;
  }

  return BTN_NONE;
}

void showMenuDisplay(String textToDisplayTop, String textToDisplayBottom) {
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

void mainMenuProcess() {

  if (menu.button == BTN_ENTER) {
    switch (menu.choice) {
      case 0:   motors.state = MOTOR_STATE_START;
        assignMenu(MENU_NONE);
        showMenuDisplay("PUSHING BUTTON!", "Please Wait...");
        break;

      case 1:
        assignMenu(MENU_DISPLAY_TIME);
        break;

      case 2: //set time
        break;
      case 3: //set alarm 1
        break;
      case 4:
        //set alarm 2
        break;
      case 5: assignMenu(MENU_MAIN_WAIT); break;
    }
  } else {

    if (menu.button == BTN_LEFT && menu.choice > 0) {
      menu.choice--;
    } else if (menu.button == BTN_RIGHT && menu.choice < 6) {
      menu.choice++;
    }

    switch (menu.choice) {
      case 0: showMenuDisplay("START MOTOR NOW", ""); break;
      case 1: showMenuDisplay("DISPLAY TIME", ""); break;
      case 2: showMenuDisplay("SET TIME", ""); break;
      case 3: showMenuDisplay("SET ALARM 1", ""); break;
      case 4: showMenuDisplay("SET ALARM 2", ""); break;
      case 5: showMenuDisplay("Exit", ""); break;
    }

  }
}

void mainMenuWait() {
  showMenuDisplay("Good Shabbos!", "");
  if (menu.button != BTN_NONE) {
    assignMenu(MENU_MAIN_PROCESS);
  }
}

void menuDisplayTime() {
  showMenuDisplay(ReadDate(), ReadTime());
  if (menu.button != BTN_NONE) {
    assignMenu(MENU_MAIN_WAIT);
  }
}

void assignMenu(int menuType) {

  menu.type = menuType;
  menu.choice = 0;

  switch (menu.type) {
    case MENU_MAIN_WAIT:
      menu.process = &mainMenuWait;
      break;
    case MENU_MAIN_PROCESS:
      menu.process = &mainMenuProcess;
      break;
    case MENU_DISPLAY_TIME:

      menu.process = &menuDisplayTime;
      break;
    default:
      //if no valid menu, leave early
      return;
  }
}

void checkForMenuButtonInput() {
  //as of right now - there's no need to read the button if there's no menu assigned
  if (menu.type != MENU_NONE) {
    unsigned char button = GetKey(analogRead(PIN_DISPLAY_BUTTON));
    if (button != menu.button) {
      menu.button = button;
      menu.process();
    }
  }

}

void setupMenuDisplay() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  assignMenu(MENU_MAIN_WAIT);
  menu.button = -1; //force first menu to display
}

/*
   MAIN
*/
void setup()
{
  Serial.begin(9600);

  setupMenuDisplay();
  setupClock();
  setupMotors();
}



void loop()
{
  checkForMenuButtonInput();
  updateMotorState();
}

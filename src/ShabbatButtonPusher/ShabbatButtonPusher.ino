#include <LiquidCrystal.h>
#include <SPI.h>
#include <SparkFunDS3234RTC.h>
#include <Servo.h>
/* GLOBALS
 *  
 */



#define PIN_MOTOR 3
#define MOTOR_STATE_NEUTRAL 0
#define MOTOR_STATE_START 1
#define MOTOR_STATE_FORWARD 2
#define MOTOR_STATE_REVERSE 3
#define MOTOR_POSITION_ORIGIN 30
#define MOTOR_POSITION_END 170

#define PIN_CLOCK 10

#define BTN_LEFT 'a'
#define BTN_RIGHT 'd'
#define BTN_UP 'w'
#define BTN_DOWN 's'
#define BTN_ENTER 'f'
#define BTN_NONE ' '

#define PIN_LCD_1 8
#define PIN_LCD_2 9
#define PIN_LCD_3 4
#define PIN_LCD_4 5
#define PIN_LCD_5 6
#define PIN_LCD_6 7

LiquidCrystal lcd(PIN_LCD_1, PIN_LCD_2, PIN_LCD_3, PIN_LCD_4, PIN_LCD_5, PIN_LCD_6);

unsigned char currentButton = 0;
void (*currentMenu)() = NULL;
int menuPos = 0;

typedef struct {
  Servo servo;
  uint8_t state;
} MotorInfo;

MotorInfo motors;



/*
 * MOTORS
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
      showMenuDisplay("Finished :D");
      currentMenu = &menuMain;
      menuPos = 0;
      
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



void doManualPush() {
  motors.state = MOTOR_STATE_START;
  currentMenu = NULL;
  showMenuDisplayFull("PUSHING BUTTON!", "Please Wait...");
}

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

void showMenuDisplayFull(String textToDisplayTop, String textToDisplayBottom) {
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

void showMenuDisplay(String textToDisplay) {
  showMenuDisplayFull("Good Shabbos!", textToDisplay);
}

void menuShowInfo() {
  switch (currentButton) {
    case BTN_LEFT: showMenuDisplay("LEFT"); break;
    case BTN_RIGHT: showMenuDisplay("RIGHT"); break;
    case BTN_UP: showMenuDisplay("UP"); break;
    case BTN_DOWN: showMenuDisplay("DOWN"); break;
    case BTN_ENTER: showMenuDisplay("ENTER"); break;
    default: showMenuDisplay(""); break;
  }
}

void menuDisplayTime() {
  showMenuDisplayFull(ReadDate(), ReadTime());
  if (currentButton != BTN_NONE) {
    currentMenu = &menuMain;
  }
}

void menuWait() {
  showMenuDisplay("");
  //placeholder - should be automatic from motor
  if (currentButton != BTN_NONE) {
    currentMenu = &menuMain;
  }
}

void menuMain() {

  if (currentButton == BTN_LEFT) {
    menuPos--;
  } else if (currentButton == BTN_RIGHT) {
    menuPos++;
  }
  if (menuPos < 0) {
    menuPos = 0;
  }
  if (menuPos > 4) {
    menuPos = 4;
  }
  switch (menuPos) {
    case 0: showMenuDisplay("MANUAL PUSH"); break;
    case 1: showMenuDisplay("DISPLAY TIME"); break;
    case 2: showMenuDisplay("ADD ROBOT"); break;
    case 3: showMenuDisplay("SHOW ROBOTS"); break;
    case 4: showMenuDisplay("DELETE ROBOT"); break;
    default: showMenuDisplay(""); break;
  }

  if (currentButton == BTN_ENTER) {
    switch (menuPos) {
      case 0: doManualPush(); break;
      case 1: currentMenu = &menuDisplayTime; menuPos = 0; break;
      case 2: showMenuDisplay("ADD ROBOT"); break;
      case 3: showMenuDisplay("SHOW ROBOTS"); break;
      case 4: showMenuDisplay("DELETE ROBOT"); break;
    }
  }
}
void updateMenuDisplay() {

  

   if(currentMenu != NULL) {
  unsigned char btn = GetKey(analogRead(A0));
  if (btn != currentButton) {
    currentButton = btn;
      currentMenu();
    }
  }

}

void setupLCD() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);


  //currentMenu = &menuWait;
  currentMenu = &menuDisplayTime;
}

/*
   MAIN
*/
void setup()
{
  Serial.begin(9600);

  setupLCD();
  setupClock();
  setupMotors();
}



void loop()
{
  updateMenuDisplay();
  updateMotorState();
}

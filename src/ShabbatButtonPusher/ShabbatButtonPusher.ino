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
#define MENU_START 1
#define MENU_MAIN_PROCESS 2
#define MENU_DISPLAY_TIME 3
#define MENU_SET_TIME 4
#define MENU_SET_TIME_SUB 5

#define BTN_NONE 0
#define BTN_LEFT 1
#define BTN_RIGHT 2
#define BTN_UP 3
#define BTN_DOWN 4
#define BTN_ENTER 5

#define TIME_DAY 0
#define TIME_DATE 1
#define TIME_MONTH 2
#define TIME_YEAR 3
#define TIME_SECOND 4
#define TIME_MINUTE 5
#define TIME_HOUR 6

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
  int subchoice;
  int button;
} MenuInfo;

MenuInfo menu;

int timeInfo[6];
/* UTILITIES

*/

void log(char *fmt, ... ) {
  
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  
  Serial.print(buf);
  Serial.print('\n');
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
      assignMenu(MENU_START);
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

void setToCurrentTime() {
  rtc.update();

  timeInfo[TIME_DAY] = rtc.day();
  timeInfo[TIME_DATE] = rtc.date();
  timeInfo[TIME_MONTH] = rtc.month();
  timeInfo[TIME_YEAR] = rtc.year();
  timeInfo[TIME_SECOND] = rtc.second();
  timeInfo[TIME_MINUTE] = rtc.minute();
  timeInfo[TIME_HOUR] = rtc.hour();
}

//=====================================
String GetCurrentDateString() {
  String temp;

  // Read the day/date:
  // temp.concat(timeInfo->dayStr);

  temp.concat(" ") ;
  temp.concat(timeInfo[TIME_DATE]);
  temp.concat("/");
  temp.concat(timeInfo[TIME_MONTH]);
  temp.concat("/") ;
  temp.concat(timeInfo[TIME_YEAR]);

  return (temp);
}

String GetCurrentTimeString() {
  String temp;
  // Read the time:
  temp.concat(timeInfo[TIME_HOUR]);
  temp.concat(":") ;
  temp.concat(timeInfo[TIME_MINUTE]);
  temp.concat(":") ;
  temp.concat(timeInfo[TIME_SECOND]);

  return (temp);
}

/*
   MENU
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
        assignMenu(MENU_SET_TIME);
        break;
      case 3: //set alarm 1
        break;
      case 4:
        //set alarm 2
        break;
      case 5: assignMenu(MENU_START); break;
    }
  } else {

    if (menu.button == BTN_LEFT && menu.choice > 0) {
      menu.choice--;
    } else if (menu.button == BTN_RIGHT && menu.choice < 5) {
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

void mainMenuStart() {
  showMenuDisplay("Good Shabbos!", "");
  if (menu.button != BTN_NONE) {
    assignMenu(MENU_MAIN_PROCESS);
  }
}

void menuDisplayTime() {
  
  setToCurrentTime();

  showMenuDisplay(GetCurrentDateString(), GetCurrentTimeString());
  if (menu.button != BTN_NONE) {
    assignMenu(MENU_START);
  }
}

String menuTimeToString(int choice) {
  switch (choice) {
    case 0: return "Day of Week";
    case 1: return "Day of Month";
    case 2: return "Month";
    case 3: return "Year";
    case 4: return "Hour";
    case 5: return "Minute";
    case 6: return "Second";
  }

  return "CANCEL";
}

void menuSetTimeInit() {
  
  setToCurrentTime();

  
  if (menu.button == BTN_ENTER) {
    
    if (menu.choice == 7) {
      assignMenu(MENU_START);

    } else {

      
      int choice = menu.choice;
      assignMenu(MENU_SET_TIME_SUB);
      menu.choice = choice;
    }

  } else {
    if (menu.button == BTN_LEFT && menu.choice > 0) {
      menu.choice--;
    } else if (menu.button == BTN_RIGHT && menu.choice < 7) {
      menu.choice++;
    }
    
    String bottomText;
    if(menu.choice < 7) {
      bottomText.concat(timeInfo[menu.choice]);
    }
    showMenuDisplay(menuTimeToString(menu.choice), bottomText);
  }

}

void menuSetTimeSub() {
  int val = timeInfo[menu.choice];
  setToCurrentTime();
  timeInfo[menu.choice] = val;
   
  if (menu.button == BTN_ENTER) {
    if (menu.subchoice != 1) {
      //TODO: SET TIME!
    }
    //log("HAPPENED IN SUB!");
     assignMenu(MENU_START);

  } else {

    if (menu.button == BTN_LEFT && menu.subchoice == 1) {
      menu.subchoice--;
    } else if (menu.button == BTN_RIGHT && menu.subchoice == 0) {
      menu.subchoice++;
    }

    String topText;
    String bottomText;

    if (menu.subchoice == 1) {
      showMenuDisplay("CANCEL", "");
    } else {


      int upperLimit = -1;

      switch (menu.choice) {
        case 0:
          upperLimit = 7;
          break;
        case 1:
          upperLimit = 31;
          break;
        case 2:
          upperLimit = 12;
          break;
        case 3:
          break;
        case 4:
          upperLimit = 24;
          break;
        case 5:
          upperLimit = 60;
          break;
        case 6:
          upperLimit = 60;
          break;
      }


      if (menu.button == BTN_UP && (upperLimit == -1 || timeInfo[menu.choice] < upperLimit)) {
        timeInfo[menu.choice]++;
      } else if (menu.button == BTN_DOWN && timeInfo[menu.choice] > 0) {
        timeInfo[menu.choice]--;
      }

      topText.concat(timeInfo[menu.choice]);
      bottomText = "Set " + menuTimeToString(menu.choice);

      showMenuDisplay(topText, bottomText);
    }

  }
}

void assignMenu(int menuType) {
  log("Assigning menu to %d", menuType);
  menu.type = menuType;
  menu.choice = 0;
  menu.subchoice = 0;
  switch (menu.type) {
    case MENU_START:
      menu.process = &mainMenuStart;
      break;
    case MENU_MAIN_PROCESS:
      menu.process = &mainMenuProcess;
      break;
    case MENU_DISPLAY_TIME:

      menu.process = &menuDisplayTime;
      break;
    case MENU_SET_TIME:
      menu.process = &menuSetTimeInit;
      break;
    case MENU_SET_TIME_SUB:
      menu.process = &menuSetTimeSub;
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
      log("BUTTON: %d MENU: %d", button, menu.type);
      menu.button = button;
      menu.process();
    }
  }

}

void setupMenuDisplay() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  assignMenu(MENU_START);
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

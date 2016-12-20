#include <LiquidCrystal.h>

#define BTN_LEFT 'a'
#define BTN_RIGHT 'd'
#define BTN_UP 'w'
#define BTN_DOWN 's'
#define BTN_ENTER 'f'
#define BTN_NONE ' '

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

unsigned char currentButton = NULL;
void (*currentMenu)() = NULL;
int menuPos = 0;



/*
 * MENU
 */
void doManualPush() {
  //placeholder
   currentMenu = &menuMovingMotors; menuPos = 0;
}

unsigned char GetKey(int value)
{
  if(value >= 850 && value <= 860) {
    return BTN_LEFT;    
  } else if(value >= 810 && value <= 820) {
    return BTN_RIGHT;
  } else if(value >= 925 && value <= 935) {
    return BTN_UP;
  } else if(value >= 900 && value <= 910) {
    return BTN_DOWN;
  } else if(value >= 610 && value <= 620) {
    return BTN_ENTER;
  }

  return BTN_NONE;
}

void showMenuDisplayFull(String textToDisplayTop, String textToDisplayBottom) {
  lcd.clear();
  if(textToDisplayTop != "") {
    lcd.setCursor(0,0);
    lcd.print(textToDisplayTop);
  }
  if(textToDisplayBottom != "") {
    lcd.setCursor(0,1);
    lcd.print(textToDisplayBottom);
  }
}

void showMenuDisplay(String textToDisplay) {
  showMenuDisplayFull("Good Shabbos!", textToDisplay);
}

void menuShowInfo() {
  switch(currentButton) {
      case BTN_LEFT: showMenuDisplay("LEFT"); break;
      case BTN_RIGHT: showMenuDisplay("RIGHT"); break;
      case BTN_UP: showMenuDisplay("UP"); break;
      case BTN_DOWN: showMenuDisplay("DOWN"); break;
      case BTN_ENTER: showMenuDisplay("ENTER"); break;
      default: showMenuDisplay(""); break;
    }
}

void menuDisplayTime() {
  showMenuDisplayFull("current date...", "current time...");
  if(currentButton != BTN_NONE) {
    currentMenu = &menuMain;
  }
}

void menuMovingMotors() {
  showMenuDisplayFull("PUSHING BUTTON!", "Please Wait...");
  //placeholder - should be automatic from motor
  if(currentButton != BTN_NONE) {
    currentMenu = &menuMain;
  }
}

void menuWait() {
  showMenuDisplay("");
  //placeholder - should be automatic from motor
  if(currentButton != BTN_NONE) {
    currentMenu = &menuMain;
  }
}

void menuMain() {

  if(currentButton == BTN_LEFT) {
    menuPos--;
  } else if(currentButton == BTN_RIGHT) {
    menuPos++;
  }
  if(menuPos < 0) {
    menuPos = 0;
  }
  if(menuPos > 4) {
    menuPos = 4;
  }
  switch(menuPos) {
      case 0: showMenuDisplay("MANUAL PUSH"); break;
      case 1: showMenuDisplay("DISPLAY TIME"); break;
      case 2: showMenuDisplay("ADD ROBOT"); break;
      case 3: showMenuDisplay("SHOW ROBOTS"); break;
      case 4: showMenuDisplay("DELETE ROBOT"); break;
      default: showMenuDisplay(""); break;
    }

   if(currentButton == BTN_ENTER) {
    switch(menuPos) {
      case 0: doManualPush(); break;
      case 1: currentMenu = &menuDisplayTime; menuPos = 0; break;
      case 2: showMenuDisplay("ADD ROBOT"); break;
      case 3: showMenuDisplay("SHOW ROBOTS"); break;
      case 4: showMenuDisplay("DELETE ROBOT"); break;
    }
   }
}
void updateButtonChoice() {
  unsigned char btn = GetKey(analogRead(A0));
  if(btn != currentButton) {
    currentButton = btn;
    if(currentMenu != NULL) {
      currentMenu();
    }
  }
  
}

void setup() 
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  Serial.begin(9600);
  
  //currentMenu = &menuWait;
  currentMenu = &menuDisplayTime;
}


 
void loop() 
{
  updateButtonChoice();
}

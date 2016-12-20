#include <stdarg.h>
#include <stdio.h>
#include <conio.h>

#define BTN_NONE 0
#define BTN_UP 1
#define BTN_LEFT 2
#define BTN_RIGHT 3
#define BTN_DOWN 4
#define BTN_ENTER 5

int currentButton = -1;

void logfmt(char *fmt, ... ) {

  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);

  printf("%s\n", buf);
}

char *buttonToString(int btn) {
  switch(btn) {
    case BTN_UP: return "UP";
    case BTN_LEFT: return "LEFT";
    case BTN_RIGHT: return "RIGHT";
    case BTN_DOWN: return "DOWN";
    case BTN_ENTER: return "ENTER";
  }
  return "NONE";
}

int getButton() {
    int keyPress = _getch();
    int btn = BTN_NONE;

    logfmt("%d", keyPress);

    switch(keyPress) {
      case 32: btn = BTN_ENTER; break;
      case 32: btn = BTN_ENTER; break;
      case 32: btn = BTN_ENTER; break;
      case 32: btn = BTN_ENTER; break;
      case 32: btn = BTN_ENTER; break;
      
    }

    return btn;
}

int main(int argc, char **argv) {
  while(1) {
    int btn = getButton();
    if(currentButton != btn) {
      currentButton = btn;
      //logfmt(buttonToString(currentButton));
    }

  }
}

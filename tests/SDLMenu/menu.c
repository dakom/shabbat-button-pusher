#include "menu.h"

int currentButton = -1;

void updateButtonPress(int btn) {
    if(currentButton != btn) {
        currentButton = btn;
        showText("Button: %s", buttonToString(btn));
    }
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
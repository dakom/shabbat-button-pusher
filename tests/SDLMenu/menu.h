#include <SDL.h>

#define BTN_NONE -1
#define BTN_UP SDLK_UP
#define BTN_LEFT SDLK_LEFT
#define BTN_RIGHT SDLK_RIGHT
#define BTN_DOWN SDLK_DOWN
#define BTN_ENTER SDLK_RETURN

void updateButtonPress(int);
void showText(char *, ...);
char *buttonToString(int);
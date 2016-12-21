#include <SDL.h>

#define NUMBER_OF_MENUS 20 // change this number to real amount for optimization

#define BTN_NONE -1
#define BTN_UP SDLK_UP
#define BTN_LEFT SDLK_LEFT
#define BTN_RIGHT SDLK_RIGHT
#define BTN_DOWN SDLK_DOWN
#define BTN_ENTER SDLK_RETURN

typedef struct MenuInfo{
  void (*showDisplay)();
  void (*processButton)();
  int horizontalIndex, verticalIndex, maxChildIndex;
  bool hasSetMaxChildren;
  struct MenuInfo *prevSibling, *nextSibling, *parent, *firstChild;
};

extern int currentButton;
extern MenuInfo *currentMenu;
extern MenuInfo menus[];

int getMaxChildIndex(MenuInfo *);
MenuInfo *getSelectedChild();
MenuInfo *createMenu(MenuInfo *, int, void (*)(), void (*)());
void assignMenu(MenuInfo *);
void setupMenus();
void updateButtonPress(int);
void showText(char *, ...);
char *buttonToString(int);


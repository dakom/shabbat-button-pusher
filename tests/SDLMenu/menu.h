#include <SDL.h>

#define NUMBER_OF_MENUS 30 // change this number to real amount for optimization

#define BTN_NONE -1
#define BTN_UP SDLK_UP
#define BTN_LEFT SDLK_LEFT
#define BTN_RIGHT SDLK_RIGHT
#define BTN_DOWN SDLK_DOWN
#define BTN_ENTER SDLK_RETURN

typedef struct MenuInfo{
  void (*showDisplay)();
  void (*onAccept)();
  int (*getVerticalStart)();
  int (*getVerticalMax)(); 
  int maxChildIndex;
  struct MenuInfo *prevSibling, *nextSibling, *parent, *firstChild;
};

extern int currentButton;
extern int verticalIndex;
extern MenuInfo *currentMenu;
extern MenuInfo menus[];

MenuInfo *getVisibleMenu();
int getMaxChildIndex(MenuInfo *);
MenuInfo *getSelectedChild();
MenuInfo *createMenu(MenuInfo *, int, void (*)(), void (*)(),  int (*)(), int (*)());
void chooseMenu(MenuInfo *);
void setupMenus();
void updateButtonPress(int);
void showText(char *, ...);
char *buttonToString(int);


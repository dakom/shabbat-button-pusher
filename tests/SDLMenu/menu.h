#include <SDL.h>

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
extern MenuInfo *rootMenu;

int getMaxChildIndex(MenuInfo *);
MenuInfo *getSelectedChild();

void assignMenu(MenuInfo *);
bool setupMenus();
void freeMenus();
void updateButtonPress(int);
void showText(char *, ...);
char *buttonToString(int);


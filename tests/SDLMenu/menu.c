#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "activeMenus.h"

int currentButton = BTN_NONE-1; //impossible value
MenuInfo *currentMenu = NULL;
MenuInfo *rootMenu = NULL;

void showshowDisplay()
{
  if (currentMenu == NULL)
  {
    showText("No Menu!");
  }
  else
  {
    currentMenu->showDisplay();
  }
}
void updateButtonPress(int btn)
{
  if (currentButton != btn)
  {
    currentButton = btn;
    if (currentMenu == NULL)
    {
      showText("No Menu!");
    }
    else
    {
      /*
       * Yeah, it's kinda ugly, but it works :)
       * Basic idea is that we are really always showing and processing the active child (hence root node is meaningless)
       * If there is no child then we show the current
       * Since child is always determined by horizontalIndex, we can do sanity checks globally here
       * Though verticalIndex upper limit should be handled by each menu itself (bottom limit is always 0)
       */

      if(currentMenu->hasSetMaxChildren == false) {
        currentMenu->maxChildIndex = getMaxChildIndex(currentMenu);
        currentMenu->hasSetMaxChildren = true;
      }

      switch (currentButton)
      {
      case BTN_UP:
        currentMenu->verticalIndex++;
        break;
      case BTN_LEFT:
        currentMenu->horizontalIndex--;
        break;
      case BTN_RIGHT:
        currentMenu->horizontalIndex++;
        break;
      case BTN_DOWN:
        currentMenu->verticalIndex--;
        break;
      }

      if(currentMenu->verticalIndex < 0) {
        currentMenu->verticalIndex = 0;
      }
      if(currentMenu->horizontalIndex < 0) {
        currentMenu->horizontalIndex = 0;
      }

      if(currentMenu->maxChildIndex != -1) {
        if(currentMenu->horizontalIndex > currentMenu->maxChildIndex) {
          currentMenu->horizontalIndex = currentMenu->maxChildIndex;
        }
        
      }

      MenuInfo *visibleMenu = getSelectedChild();
      if(visibleMenu == NULL) {
        visibleMenu = currentMenu;
      }

      //menus don't need to always do something... can be just informative
      if(visibleMenu->processButton != NULL) {
        visibleMenu->processButton();
      }

      //they must always show something, however
      visibleMenu->showDisplay();
      
    }
  }
}


int getMaxChildIndex(MenuInfo *parent) {
  int i = -1;
  if(parent->firstChild != NULL) {
    MenuInfo *child = parent->firstChild;
    for(i = 0; child->nextSibling != NULL; i++) {
      child = child->nextSibling;
    }
  }

  return i;
}

MenuInfo *getSelectedChild() {
  MenuInfo *parent = currentMenu;

  if(parent->firstChild == NULL) {
    return NULL;
  }

  MenuInfo *child = parent->firstChild;
  for(int i = 0; i < currentMenu->horizontalIndex && child->nextSibling != NULL; i++) {
    child = child->nextSibling;
  }
  return child;
}

char *buttonToString(int btn)
{
  switch (btn)
  {
  case BTN_UP:
    return "UP";
  case BTN_LEFT:
    return "LEFT";
  case BTN_RIGHT:
    return "RIGHT";
  case BTN_DOWN:
    return "DOWN";
  case BTN_ENTER:
    return "ENTER";
  }
  return "NONE";
}



void assignMenu(MenuInfo *menu)
{
  currentMenu = menu;
  currentMenu->horizontalIndex = 0;
  currentMenu->verticalIndex = 0;
}

MenuInfo *createMenu(MenuInfo *parent, void (*showDisplay)(), void (*processButton)())
{

  MenuInfo *menu = (MenuInfo *)malloc(sizeof(MenuInfo));

  if (menu == NULL)
  {
    return NULL;
  }

  menu->prevSibling = menu->nextSibling = menu->parent = menu->firstChild = NULL;
  menu->showDisplay = showDisplay;
  menu->processButton = processButton;
  menu->horizontalIndex = 0;
  menu->verticalIndex = 0;
  menu->maxChildIndex = -1;
 menu->hasSetMaxChildren = false;

  if (parent != NULL)
  {
    menu->parent = parent;

    if (parent->firstChild == NULL)
    {
      parent->firstChild = menu;
    }
    else
    {
      MenuInfo *sibling = parent->firstChild;
      while (sibling->nextSibling != NULL)
      {
        sibling = sibling->nextSibling;
      }
      sibling->nextSibling = menu;
      menu->prevSibling = sibling;
    }
  }

  return menu;
}

bool setupMenus()
{
  MenuInfo *menu;

  menu = createMenu(NULL, NULL, NULL);
  if (menu == NULL)
  {
    return false;
  }
  rootMenu = menu;

  menu = createMenu(menu, &mainMenuShowDisplay, &mainMenuProcessButton);
  if (menu == NULL)
  {
    return false;
  }

  menu = createMenu(menu, &child1ShowDisplay, NULL);
  if (menu == NULL)
  {
    return false;
  }

  menu = createMenu(menu->parent, &child2ShowDisplay, NULL);
  if (menu == NULL)
  {
    return false;
  }

   menu = createMenu(menu->parent, &child3ShowDisplay, &child3ProcessButton);
  if (menu == NULL)
  {
    return false;
  }

  return true;
}

void freeMenus()
{
  //need to walk the list from the root and free it!
  //Though for our purposes, on Arduino, not really :P
}

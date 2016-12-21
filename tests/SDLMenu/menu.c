#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "activeMenus.h"

MenuInfo menus[NUMBER_OF_MENUS];
int currentButton = BTN_NONE - 1; //impossible value
MenuInfo *currentMenu = NULL;
MenuInfo *rootMenu = NULL;
int horizontalIndex = 0;
int verticalIndex = 0;

void updateButtonPress(int btn)
{
  if (currentButton != btn)
  {
    currentButton = btn;
    /*
       * Yeah, it's kinda ugly, but it works :)
       * We are really always showing and processing the active child, not really the active menu
       * If there is no child then we show the current as a fallback
       * Since child is always determined by horizontalIndex, we can do sanity checks globally here
       * verticalIndex upper limit should be handled by each menu itself (bottom limit is always 0)
       */

    

    switch (currentButton)
    {
    case BTN_UP:
      verticalIndex++;
      break;
    case BTN_LEFT:
      horizontalIndex--;
      break;
    case BTN_RIGHT:
      horizontalIndex++;
      break;
    case BTN_DOWN:
      verticalIndex--;
      break;
    }

    if (verticalIndex < 0)
    {
      verticalIndex = 0;
    }
    if (horizontalIndex < 0)
    {
      horizontalIndex = 0;
    }

    

    if (currentMenu->maxChildIndex == -1)
    {
      //once per menu, delayed till we need it here
      currentMenu->maxChildIndex = getMaxChildIndex(currentMenu);
    }

    if (horizontalIndex > currentMenu->maxChildIndex) {
        horizontalIndex = currentMenu->maxChildIndex;
    }

    MenuInfo *visibleMenu = getSelectedChild();

    if (visibleMenu == NULL)
    {
      visibleMenu = currentMenu;
    }

    int verticalMax = 0;

    if(visibleMenu->getVerticalMax != NULL) {
      verticalMax = visibleMenu->getVerticalMax();  
    }

    if(verticalIndex > verticalMax) {
      verticalIndex = verticalMax;
    }

    if (currentButton == BTN_ENTER && visibleMenu->onAccept != NULL) {
      visibleMenu->onAccept();
    }

    //they must always show something, however
    visibleMenu->showDisplay();
  }
}

int getMaxChildIndex(MenuInfo *parent)
{
  int i = 0;
  if (parent->firstChild != NULL)
  {
    MenuInfo *child = parent->firstChild;
    for (i = 0; child->nextSibling != NULL; i++)
    {
      child = child->nextSibling;
    }
  }

  return i;
}

MenuInfo *getSelectedChild()
{
  MenuInfo *parent = currentMenu;

  if (parent->firstChild == NULL)
  {
    return NULL;
  }

  MenuInfo *child = parent->firstChild;
  for (int i = 0; i < horizontalIndex && child->nextSibling != NULL; i++)
  {
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

void chooseMenu(MenuInfo *menu)
{
  currentMenu = menu;
  horizontalIndex = 0;
  verticalIndex = 0;
}

MenuInfo *createMenu(MenuInfo *parent, int index, void (*showDisplay)(), void (*onAccept)(), int (*getVerticalMax)())
{

  MenuInfo *menu = &menus[index];

  menu->prevSibling = menu->nextSibling = menu->parent = menu->firstChild = NULL;
  menu->showDisplay = showDisplay;
  menu->onAccept = onAccept;
  menu->getVerticalMax = getVerticalMax;
  menu->maxChildIndex = -1;
  
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
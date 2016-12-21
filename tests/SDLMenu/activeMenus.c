#include <stdlib.h>
#include <stdio.h>
#include "menu.h"

void mainMenuShowDisplay()
{
  showText("Good Shabbos!");
}

void mainMenuProcessButton()
{
  if (currentButton != BTN_NONE)
  {
      assignMenu(getSelectedChild());
  }
}

void child1ShowDisplay()
{
  showText("Child 1");
}


void child2ShowDisplay()
{
  showText("Child 2");
}


void child3ShowDisplay()
{
  showText("CANCEL");
}

void child3ProcessButton()
{
  if (currentButton == BTN_ENTER)
  {
      assignMenu(rootMenu);
  }
}

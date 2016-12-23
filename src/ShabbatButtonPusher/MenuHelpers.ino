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


void setVisibleMenu() {
  visibleMenu = getSelectedChild();

    if (visibleMenu == NULL)
    {
      visibleMenu = currentMenu;
    }

}


void chooseMenu(MenuInfo *menu)
{
  currentMenu = menu;
  horizontalIndex = 0;

  verticalIndex = 1;

  setVisibleMenu();

  if(visibleMenu != NULL) {
    if(visibleMenu->getVerticalStart != NULL) {
      verticalIndex = visibleMenu->getVerticalStart();
    }
  }
  
}

/*
 * Called in main loop
 */
void updateButtonPress()
{
  int btn = GetKey(analogRead(PIN_DISPLAY_BUTTON));
 
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

    if (currentMenu->maxChildIndex == -1)
    {
      //once per menu, delayed till we need it here
      currentMenu->maxChildIndex = getMaxChildIndex(currentMenu);
    }

    if (horizontalIndex < 0)
    {
      horizontalIndex = 0;
    }
    if (horizontalIndex > currentMenu->maxChildIndex) {
        horizontalIndex = currentMenu->maxChildIndex;
    }

    setVisibleMenu();

    int verticalMax = 0;

    if(visibleMenu->getVerticalMax != NULL) {
      verticalMax = visibleMenu->getVerticalMax();  
    }

    if (verticalIndex < 1)
    {
      verticalIndex = verticalMax;
    }

    if(verticalIndex > verticalMax) {
      verticalIndex = 1;
    }
    
    if (currentButton == BTN_ENTER && visibleMenu->onAccept != NULL) {
      visibleMenu->onAccept();
    }

    //they must always show something, however
    visibleMenu->showDisplay();
  }
}


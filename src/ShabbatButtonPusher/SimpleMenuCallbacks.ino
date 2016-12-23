/*
 * Generic
 */

void genericCancelDisplay()
{
  showText("CANCEL");
}

void genericCancelProcessButton()
{
  chooseMenu(&menus[0]);
}

void genericAcceptChildProcessButton()
{
  chooseMenu(getSelectedChild());
}

void genericUpdateVisibleDisplay() {
  visibleMenu->showDisplay();
}

void genericSelectThroughAnyButton()
{
  if (currentButton != BTN_NONE)
  {
      chooseMenu(getSelectedChild());
  }
}


/*
 * Display only
 */
void mainMenuShowDisplay()
{
  showText("Good Shabbos!");
}


void mainChildStartMotorDisplay()
{
  showText("Start Motor Now");
}


void mainChildDisplayTimeDisplay()
{
  showText("Display Time");
}

void mainChildSetTimeDisplay()
{
  showText("Set Time");
}

void mainChildSetAlarmDisplay()
{
  showText("Set Alarm");
}

void mainChildToggleShabbosDisplay() {
  showText("Toggle Shabbos Mode");
}

void toggleShabbosOnDisplay() {
  showText("Turn on Shabbos (1:00PM)");
}

void toggleShabbosOffDisplay() {
  showText("Turn off Shabbos");
}




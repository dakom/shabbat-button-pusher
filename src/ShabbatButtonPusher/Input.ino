int GetKey(int value)
{

  int btn = BTN_NONE;

  if (value > 600 && value < 1000) {
    if (value < 650) {
      btn = BTN_ENTER;
    } else if (value < 840) {
      btn = BTN_RIGHT;
    } else if (value < 890) {
      btn = BTN_LEFT;
    } else if (value < 920) {
      btn = BTN_DOWN;
    } else if (value < 950) {
      btn = BTN_UP;
    }
  }

  return btn;
}

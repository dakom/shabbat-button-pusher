void setupDisplay() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void clearDisplay() {
  lcd.clear();
}

void showFullDisplay(String textToDisplayTop, String textToDisplayBottom) {
  clearDisplay();
  
  if (textToDisplayTop != "") {
    lcd.setCursor(0, 0);
    lcd.print(textToDisplayTop);
  }
  if (textToDisplayBottom != "") {
    lcd.setCursor(0, 1);
    lcd.print(textToDisplayBottom);
  }
}

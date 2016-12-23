
void log(char *fmt, ... ) {
  
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  
  Serial.print(buf);
  Serial.print('\n');
}


String GetFormattedString(char *fmt, ... ) {
  
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);

  return buf;
}

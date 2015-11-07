#include "SerialLogger.h"

void setup()
{
  Serial.begin(9600);
  Log.init(&Serial, LOG_LEVEL_VERBOSE);
}

void loop()
{
  Log.Info(F("Hello there"));
  Log.Verbose(F("I can do this as well '%F'"),F("Big nasty Progmem string"));
  Log.Debug(F("I can print HEX '%X'"),123);
  Log.Debug(F("Or binary '%b'"),123);
  delay(5000);
}

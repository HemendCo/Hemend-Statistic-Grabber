#include <Arduino.h>
#include <RFID.h>
#include <SerialModule.h>
#include <SPI.h>
#include <TimeModule.h>
#define MODULE_COUNT 2
RFID rfid_module = RFID();
TimeModule time_module = TimeModule();
SerialModule serial = SerialModule();
void initModules();
void updateModules();

void setup()
{
  SPI.begin();
  initModules();
}

void loop()
{
  updateModules();
}

void initModules()
{
  serial.init();
  rfid_module.init();
  // Time t(2022, 4, 3, 3, 38, 4, Time::kFriday);
  // time_module.init(t);
}
void updateModules()
{
  serial.update();
  rfid_module.update();
  time_module.update();
  delay(1000);
}
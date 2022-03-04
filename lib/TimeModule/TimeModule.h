
#ifndef TimeModule_h
#define TimeModule_h

#include "Arduino.h"
#include <BaseModule.h>
#include <String.h>
#include "DS1302.h"

// Set the appropriate digital I/O pin connections. These are the pin
// assignments for the Arduino as well for as the DS1302 chip. See the DS1302
// datasheet:
//
//   http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
#define RESET_PIN 4 // Chip Enable
#define DATA_PIN 3  // Input/Output
#define CLOCK_PIN 2 // Serial Clock
const DS1302 _CLOCK_MODULE = DS1302(RESET_PIN, DATA_PIN, CLOCK_PIN);
class TimeModule : public BaseModule
{
public:
    TimeModule();
    void doCommand(String);
    void init(Time);
    void update();

private:
    String dayAsString(const Time::Day day);
};

#endif

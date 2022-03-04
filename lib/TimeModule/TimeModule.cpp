#include "Arduino.h"
#include "TimeModule.h"
#include <String.h>

TimeModule::TimeModule()
{
}

void TimeModule::doCommand(String command)
{
}
void TimeModule::update()
{
    Time t = _CLOCK_MODULE.time();

    // Name the day of the week.
    const String day = dayAsString(t.day);

    // Format the time and date and insert into the temporary buffer.
    char buf[50];
    snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
             day.c_str(),
             t.yr, t.mon, t.date,
             t.hr, t.min, t.sec);

    // Print the formatted string to serial so we can see the time.
    Serial.println(buf);
}
void TimeModule::init(Time currentTime)
{
    _CLOCK_MODULE.writeProtect(false);
    _CLOCK_MODULE.halt(false);
    _CLOCK_MODULE.time(currentTime);
}
String TimeModule::dayAsString(const Time::Day day)
{
    switch (day)
    {
    case Time::kSunday:
        return "Sunday";
    case Time::kMonday:
        return "Monday";
    case Time::kTuesday:
        return "Tuesday";
    case Time::kWednesday:
        return "Wednesday";
    case Time::kThursday:
        return "Thursday";
    case Time::kFriday:
        return "Friday";
    case Time::kSaturday:
        return "Saturday";
    }
    return "(unknown day)";
}

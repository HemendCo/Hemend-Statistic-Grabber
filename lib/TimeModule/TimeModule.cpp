#include "Arduino.h"
#include "TimeModule.h"
#include <String.h>
TimeModule::TimeModule()
{
}

void TimeModule::doCommand(String command)
{
}
String TimeModule::update()
{
    Time t = _CLOCK_MODULE.time();

    // Format the time and date and insert into the temporary buffer.
    char buf[50];
    snprintf(buf, sizeof(buf), "%s%04d-%02d-%02d %02d:%02d:%02d",
             "",
             t.yr, t.mon, t.date,
             t.hr, t.min, t.sec);

    // Print the formatted string to serial so we can see the time.
    return buf;
}
void TimeModule::init(Time currentTime)
{
    _CLOCK_MODULE.writeProtect(false);
    _CLOCK_MODULE.halt(false);
    _CLOCK_MODULE.time(currentTime);
}
int TimeModule::toEpoch(int year, int month, int day, int hour, int minutes, int seconds)
{
    int yr = year - 1970;
    int mn = month - 1;
    int d = day - 1;
    Serial.println(yr);
    Serial.println(mn);
    Serial.println(d);
    Serial.println((yr * 31556926) + (mn * 2629743) + (d * 86400) + (hour * 3600) + (minutes * 60) + seconds);
    int result = (yr * 31556926) + (mn * 2629743) + (d * 86400) + (hour * 3600) + (minutes * 60) + seconds;
    return result;
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

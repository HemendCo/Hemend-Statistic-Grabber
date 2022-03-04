#include "Arduino.h"
#include "SerialModule.h"
#include <String.h>
SerialModule::SerialModule()
{
}

void SerialModule::doCommand(String command)
{
}
void printOut(String message)
{
    Serial.println(message);
}
void SerialModule::init()
{
    // Start serial in given baud
    Serial.begin(SERIAL_BAUD);
    Serial.setTimeout(15);
    // Forcing serial to begin before next command
    while (!Serial)
        ;
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void SerialModule::update()
{
}

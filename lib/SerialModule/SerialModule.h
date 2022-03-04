
#ifndef SerialModule_h
#define SerialModule_h

#include "Arduino.h"
#include <BaseModule.h>
#include <String.h>
#include <MFRC522.h>

#define SERIAL_BAUD 9600

class SerialModule : public BaseModule
{
public:
    SerialModule();
    void doCommand(String);
    void init();
    void update();

private:
};

#endif

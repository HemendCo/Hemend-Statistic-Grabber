#include "Arduino.h"
#include "RFID.h"
#include <String.h>
RFID::RFID()
{
    rfid = MFRC522(SS_PIN, RST_PIN);
}

void RFID::doCommand(String command)
{
}

void RFID::init()
{
    rfid.PCD_Init();
    delay(10);                      // Optional delay. Some board do need more time after init to be ready, see Readme
    rfid.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
}

MFRC522::Uid RFID::update()
{
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    {
        auto null = MFRC522::Uid();
        null.size = 0;
        null.sak = 0x00;
        return null;
    }

    return rfid.uid;

    // Dump debug info about the card; PICC_HaltA() is automatically called
    // rfid.PICC_DumpToSerial(&(rfid.uid));
}

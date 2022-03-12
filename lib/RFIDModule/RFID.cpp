#include "Arduino.h"
#include "RFID.h"
RFID::RFID()
{
    rfid = MFRC522(SS_PIN_RFID, RST_PIN);
}

void RFID::doCommand(String command)
{
}

void RFID::init()
{
    rfid.PCD_Init();
    rfid.PICC_HaltA(); // halt PICC
    rfid.PCD_StopCrypto1();
}

MFRC522::Uid RFID::getCurrentTag()
{

    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    {
        auto null = MFRC522::Uid();
        null.size = 0;
        null.sak = 0x00;

        rfid.PICC_HaltA(); // halt PICC
        rfid.PCD_StopCrypto1();
        return null;
    }

    // Serial.print(F("Card UID:"));
    // for (byte i = 0; i < rfid.uid.size; i++)
    // {
    //     Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //     Serial.print(rfid.uid.uidByte[i], HEX);
    // }

    return rfid.uid;

    // Dump debug info about the card; PICC_HaltA() is automatically called
    // rfid.PICC_DumpToSerial(&(rfid.uid));
}

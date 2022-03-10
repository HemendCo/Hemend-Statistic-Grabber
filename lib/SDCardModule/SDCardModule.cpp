#include "Arduino.h"
#include "SDCardModule.h"
#include <String.h>
SDCardModule::SDCardModule()
{
}

void SDCardModule::doCommand(String command)
{
}
void SDCardModule::update()
{
}
void SDCardModule::init()
{

    if (!SD.begin(SD_CARD_SELECT, SD_CARD_MOSI, SD_CARD_MISO, SD_CARD_CLOCK))
    {
        Serial.println("Card Error");
    }
}

String SDCardModule::readLog()
{
    File dataFile = SD.open(LOG_FILE_PATH.c_str());
    if (dataFile)
    {
        String result = "";
        // while (dataFile.available())
        // {

        //     result += (char)dataFile.read();
        // }
        Serial.println(dataFile.readString());
        dataFile.seek(400);
        Serial.println(dataFile.readString());
        dataFile.close();
        return result;
    }
    else
    {
        return "error";
    }
}
void SDCardModule::writeToLog(String info)
{
    File dataFile = SD.open(LOG_FILE_PATH.c_str(), FILE_WRITE);
    if (dataFile)
    {
        dataFile.println(info);
        dataFile.close();
    }
    else
    {
        Serial.println("ERROR");
    }
}
void SDCardModule::writeTo(String fileName, String info)
{
    File dataFile = SD.open(fileName.c_str(), FILE_WRITE);
    if (dataFile)
    {
        dataFile.println(info);
        dataFile.close();
    }
    else
    {
    }
}
void SDCardModule::deleteLog()
{
    String backUp = readLog();
    writeTo(LOG_BACKUP_PATH, backUp);
    SD.remove((char *)LOG_FILE_PATH.c_str());
}
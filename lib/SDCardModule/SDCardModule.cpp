#include "Arduino.h"
#include "SDCardModule.h"
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

    SD.mkdir((char *)LOG_FILE_BACKUP_DIR);
}

void SDCardModule::loadLogToSerial()
{
    Serial.println("LOGFILE_BEGIN");

    File dataFile = SD.open(LOG_FILE_PATH);
    dataFile.setTimeout(500);
    if (dataFile)
    {
        char current = '0';
        do
        {
            current = (char)dataFile.read();
            if ((int)current == -1)
                break;
            Serial.print(current);
            // count++;
            dataFile.flush();
        } while ((int)current != -1);
        Serial.println("LOGFILE_END");
    }
    else
    {

        Serial.println("LOGFILE_ERROR");
    }
    // dataFile.flush();
    dataFile.close();
}
void SDCardModule::writeToLog(String info)
{
    writeTo(LOG_FILE_PATH, info);

    writeTo(LOG_FILE_BACKUP_FILE_PATH, info);

    // File dataFile = SD.open(LOG_FILE_PATH.c_str(), FILE_WRITE);
    // if (dataFile)
    // {
    //     dataFile.println(info);
    //     dataFile.close();
    // }
    // else
    // {
    //     Serial.println("ERROR");
    // }
}
void SDCardModule::writeTo(const char *fileName, String info)
{
    File dataFile = SD.open(fileName, FILE_WRITE);
    if (dataFile)
    {
        dataFile.println(info);
    }
    else
    {
        Serial.println("ERROR on " + String(fileName));
    }
    // dataFile.flush();
    delay(35);
    dataFile.close();
}
void SDCardModule::deleteLog()
{
    SD.remove((char *)LOG_FILE_PATH);
}


#ifndef SDCardModule_h
#define SDCardModule_h

#include "Arduino.h"
#include <BaseModule.h>
#include <String.h>
#include <SD.h>
#define SD_CARD_SELECT 5
#define SD_CARD_MOSI 6
#define SD_CARD_MISO 7
#define SD_CARD_CLOCK 8
const String LOG_FILE_PATH = "datalog.txt";
const String LOG_BACKUP_BACKUP_PATH = "datalog.back";

class SDCardModule : public BaseModule
{
public:
    SDCardModule();
    void doCommand(String);
    void init();
    void update();
    String readLog();
    void writeToLog(String info);
    void writeTo(String fileName, String info);
    void deleteLog();

private:
};

#endif

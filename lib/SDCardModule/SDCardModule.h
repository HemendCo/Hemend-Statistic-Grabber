
#ifndef SDCardModule_h
#define SDCardModule_h

#include "Arduino.h"
#include <BaseModule.h>
#include <SD.h>
#define SD_CARD_SELECT 5
#define SD_CARD_MOSI 6
#define SD_CARD_MISO 7
#define SD_CARD_CLOCK 8
#define LOG_FILE_PATH "datalog.txt"
#define LOG_FILE_BACKUP_DIR "backup"
#define LOG_FILE_BACKUP_FILE_PATH "backup/datalog.txt"

class SDCardModule : public BaseModule
{
public:
    SDCardModule();
    void doCommand(String);
    void init();
    void update();
    void loadLogToSerial();
    void writeToLog(String info);
    bool writeTo(const char *fileName, String info);
    void deleteLog();

private:
};

#endif

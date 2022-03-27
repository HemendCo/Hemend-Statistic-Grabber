import serial
import pandas as pd
from configparser import ConfigParser as cfgParser
from datetime import datetime
from persiantools.jdatetime import JalaliDateTime as jd

import time
import sys
# import getch

configDir = './config/'
settingsPath = configDir+'settings.ini'
shouldDelete = '--dellog' in sys.argv

print('will delete file ' + str(shouldDelete))
print('loading settings')
settings = cfgParser()
settings.read(settingsPath)


# load output file path and user mapper
PLAIN_OUTPUT_FILE = settings.get('Path', 'PLAIN_OUTPUT_FILE')
BACKUP_OUTPUT_FILE = settings.get('Path', 'BACKUP_OUTPUT_FILE')
FINAL_OUTPUT_FILE = settings.get('Path', 'FINAL_OUTPUT_FILE')
USERS_DICTIONARY_FILE = settings.get('Path', 'USERS_DICTIONARY_FILE')
usersMap = pd.read_csv(USERS_DICTIONARY_FILE)

# board info
SERIAL_PORT_NAME = settings.get('SerialConfig', 'SERIAL_PORT_NAME')
SERIAL_BAUD_RATE = settings.getint('SerialConfig', 'SERIAL_BAUD_RATE')

# commands
READ_FILE_COMMAND = settings.get('Commands', 'READ_FILE_COMMAND')
DELETE_FILE_COMMAND = settings.get('Commands', 'DELETE_FILE_COMMAND')
SET_TIME_COMMAND = settings.get('Commands', 'SET_TIME_COMMAND')

# output formatter settings
TEXT_FORMATTER_PREFIX = settings.get('Formatting', 'PREFIX_TEXT')
TEXT_FORMATTER_SUFFIX = settings.get('Formatting', 'SUFFIX_TEXT')
TEXT_FORMATTER_BOOT = settings.get('Formatting', 'BOOT_TEXT')
TEXT_FORMATTER_ERROR = settings.get('Formatting', 'ERROR_TEXT')
BASE_ENCODING = 'utf-8'

print('connecting to board')
serialClient = serial.Serial(SERIAL_PORT_NAME, SERIAL_BAUD_RATE)


def sendCommand(command):
    b = bytes(command, BASE_ENCODING)
    serialClient.write(b)


waitingForBoot = True
while(waitingForBoot):
    line = str(serialClient.readline().decode(BASE_ENCODING))
    if line.rfind(TEXT_FORMATTER_BOOT) >= 0:
        waitingForBoot = False
print('Boot Completed')
time.sleep(1)

print('sync time')
timeCommand = SET_TIME_COMMAND.replace('#year', str(datetime.now().year).rjust(4, '0')).replace('#month', str(datetime.now().month).rjust(2, '0')).replace('#day', str(datetime.now().day).rjust(2, '0')).replace('#hour', str(
    datetime.now().hour).rjust(2, '0')).replace('#minute', str(datetime.now().minute).rjust(2, '0')).replace('#seconds', str(datetime.now().second).rjust(2, '0')).replace('12OfWeek', str((datetime.now().weekday()+1) % 7+1))
sendCommand(timeCommand)
time.sleep(1)

print('reading log file')
sendCommand(READ_FILE_COMMAND)
fileReader = True
logData = ''
while(fileReader):
    line = str(serialClient.readline().decode(BASE_ENCODING))
    logData += line
    print(line)
    if line.rfind(TEXT_FORMATTER_SUFFIX) >= 0:
        fileReader = False
    if line.rfind(TEXT_FORMATTER_ERROR) >= 0:
        raise Exception(
            "File is not found or there is error in log file reader method if you know there should be a file please restart the board otherwise use a tag on board and try again")
logData = logData.replace(TEXT_FORMATTER_BOOT, '')
logData = logData.replace(TEXT_FORMATTER_PREFIX, '')
logData = logData.replace(TEXT_FORMATTER_SUFFIX, '')
logData.replace('\r\n', '\n')
logData.replace('\r', '')
time.sleep(1)

if shouldDelete:
    print('delete log')
    sendCommand(DELETE_FILE_COMMAND)
else:
    print('skipping delete section')


print('writing to backup')
f = open(BACKUP_OUTPUT_FILE, "a")
f.write(logData)
f.close()


def formatDateTime(input):
    date = datetime.fromisoformat(input)
    return (jd.to_jalali(date).isoformat().replace('T', ' '))


print('final formatting')

for index, row in usersMap.iterrows():
    logData = logData.replace(row['BOARD_ID'], row['USER_ID'])
logData = logData.replace('\n\n', '\n')


f = open(PLAIN_OUTPUT_FILE, "w")
f.write('ID,DATE_TIME'+logData)
f.close()


# reformat data
data = pd.read_csv(PLAIN_OUTPUT_FILE)
for index, row in data.iterrows():
    row['DATE_TIME'] = formatDateTime(row['DATE_TIME'])
data.to_csv(FINAL_OUTPUT_FILE)


# f = open(FINAL_OUTPUT_FILE, "w")
# f.write('ID,DATE_TIME'+logData)
# f.close()

print('done! press any key to close.')
# getch.getch()

import serial
import pandas as pd
from configparser import ConfigParser as cfgParser
from persiantools.jdatetime import JalaliDateTime as jd
from datetime import datetime
import time
import json
import requests
def delay(seconds: float):
    time.sleep(seconds)


configDir='./config/'
settingsPath=configDir+'settings.ini'
#loading information
settings=cfgParser()
settings.read(settingsPath)

dateTimeTemp = jd.now().isoformat()

# load output file path and user mapper
PLAIN_OUTPUT_CSV_FILE = settings.get(
    'Path', 'PLAIN_OUTPUT_CSV_FILE').replace('%dt', dateTimeTemp)
BACKUP_OUTPUT_CSV_FILE = settings.get(
    'Path', 'BACKUP_OUTPUT_CSV_FILE').replace('%dt', dateTimeTemp)
FINAL_OUTPUT_CSV_FILE = settings.get(
    'Path', 'FINAL_OUTPUT_CSV_FILE').replace('%dt', dateTimeTemp)
# FINAL_OUTPUT_XLSX_FILE = settings.get(
#     'Path', 'FINAL_OUTPUT_XLSX_FILE').replace('%dt', dateTimeTemp)
USERS_DICTIONARY_CSV_FILE = settings.get('Path', 'USERS_DICTIONARY_CSV_FILE')
usersMap = pd.read_csv(USERS_DICTIONARY_CSV_FILE)

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


usersMap=pd.read_csv(USERS_DICTIONARY_CSV_FILE)


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
delay(1)

print('sync time')
timeCommand = SET_TIME_COMMAND.replace('#year', str(datetime.now().year).rjust(4, '0')).replace('#month', str(datetime.now().month).rjust(2, '0')).replace('#day', str(datetime.now().day).rjust(2, '0')).replace('#hour', str(
    datetime.now().hour).rjust(2, '0')).replace('#minute', str(datetime.now().minute).rjust(2, '0')).replace('#seconds', str(datetime.now().second).rjust(2, '0')).replace('12OfWeek', str((datetime.now().weekday()+1) % 7+1))
sendCommand(timeCommand)
delay(1)

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
delay(1)

import sys
for i in sys.argv:
    print('sending command : '+i)
    sendCommand(i)
    delay(1)
print('writing to backup')
f = open(BACKUP_OUTPUT_CSV_FILE, "a")
f.write(logData)
f.close()
def formatDateTime(input):
    date = datetime.fromisoformat(input)
    return (jd.to_jalali(date).isoformat().replace('T', ' '))

print('final formatting')

for index, row in usersMap.iterrows():
    logData = logData.replace(row['BOARD_ID'], row['USER_ID'])
logData = logData.replace('\n\n', '\n')
logData = logData.replace(',\n', '\n')

f = open(FINAL_OUTPUT_CSV_FILE, "w")
f.write('id,date_time'+logData)
f.close()
# data = pd.read_csv(PLAIN_OUTPUT_CSV_FILE)
# data.to_csv(FINAL_OUTPUT_CSV_FILE)
dateTimeKey='date_time'
idKey='id'
presentKey='is_in'
data=pd.read_csv('result.csv')
data.insert(len(data.columns), presentKey, 0)
data.head()
def getKey(row):
    date=row[dateTimeKey].split(' ')[0]
    return date+row[idKey]
userIn=[]
for i in range(len(data)):
    item=data.iloc[i]
    
    if(getKey(item) not in userIn):
        data.iat[i, data.columns.get_loc(presentKey)] =1
        userIn.append(getKey(item))
    else:
        data.iat[i, data.columns.get_loc(presentKey)] =0
        userIn.remove(getKey(item))
data.head(20)
data.to_csv('test.csv')
jsonData= {
    'data':json.loads(data.to_json(orient ='records'))
}
# print(jsonData)
url = 'https://staff.hemend.com/api/attendance'
x = requests.post(url, json =jsonData)
print(x.text)
delay(500)
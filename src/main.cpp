#include <Arduino.h>

#include <RFID.h>
#include <SerialModule.h>
#include <SPI.h>
#include <TimeModule.h>
#include <SDCardModule.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BEEP_PIN A0

RFID rfid_module = RFID();
SDCardModule sdcard_module = SDCardModule();
TimeModule time_module = TimeModule();
SerialModule serial = SerialModule();
LiquidCrystal_I2C lcd(0x27, 20, 4);
void initModules();
void updateModules();
void handleSerial();
String mapIDToName(String);
String s(byte *, int);
void beep();

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(500);
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

  SPI.begin();
  initModules();
}

void loop()
{
  updateModules();
}

void initModules()
{
  sdcard_module.init();
  rfid_module.init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("BOOT");
  delay(500);
  lcd.noBacklight();
  Serial.println("BOOT");
}
void updateModules()
{
  handleSerial();

  auto cardPresent = rfid_module.getCurrentTag();
  if (cardPresent.size != 0)
  {
    beep();
    String uid = s(cardPresent.uidByte, cardPresent.size);
    String name = mapIDToName(uid);

    lcd.backlight();
    lcd.clear();
    lcd.print(name);
    lcd.setCursor(0, 1);
    lcd.print(time_module.getCurrentTime());
    delay(150);
    sdcard_module.writeToLog(name + "," + time_module.getCurrentTime());
    delay(2000);
  }
  else
  {
    lcd.noBacklight();
    lcd.clear();
    lcd.println(time_module.getCurrentTime());
  }
  delay(35);
}

void handleSerial()
{
  String serialCommand = Serial.readString();
  // Serial.println(serialCommand);
  if (serialCommand.startsWith("log:"))
  {
    serialCommand.remove(0, 4);
    if (serialCommand.startsWith("get"))
    {
      sdcard_module.loadLogToSerial();
    }
    if (serialCommand.startsWith("delete"))
    {
      sdcard_module.deleteLog();
    }
  }
  else if (serialCommand.startsWith("time:"))
  {
    serialCommand.remove(0, 5);
    if (serialCommand.startsWith("get"))
    {
      Serial.println(time_module.getCurrentTime());
    }
    if (serialCommand.startsWith("set:"))
    {
      serialCommand.remove(0, 4);

      int year = atoi(serialCommand.substring(0, 4).c_str());
      serialCommand.remove(0, 4);
      int month = atoi(serialCommand.substring(0, 2).c_str());
      serialCommand.remove(0, 2);
      int day = atoi(serialCommand.substring(0, 2).c_str());
      serialCommand.remove(0, 2);
      int hour = atoi(serialCommand.substring(0, 2).c_str());
      serialCommand.remove(0, 2);
      int minute = atoi(serialCommand.substring(0, 2).c_str());
      serialCommand.remove(0, 2);
      int seconds = atoi(serialCommand.substring(0, 2).c_str());
      serialCommand.remove(0, 2);
      int dayOfWeek = atoi(serialCommand.substring(0, 1).c_str()); // sunday=1,sat=7
      serialCommand.remove(0, 1);
      Time t(year, month, day, hour, minute, seconds, (Time::Day)dayOfWeek);
      time_module.init(t);
    }
  }
}

String mapIDToName(String uid)
{
  String result;
  if (uid == "B3FB3797")
  {
    result = "A. Forootan";
  }
  else if (uid == "73659297")
  {
    result = "L. Javadi";
  }
  else if (uid == "396458C2")
  {
    result = "B.Arast";
  }
  else if (uid == "6CE1D55B")
  {
    result = "A. Hoseini";
  }
  else if (uid == "63034997")
  {
    result = "M. Fallahnejad";
  }
  else if (uid == "03796897")
  {
    result = "A. Alikhani";
  }
  else if (uid == "C3C88197")
  {
    result = "K. Pazooki";
  }
  else if (uid == "632A1497")
  {
    result = "A. Aryaeifar";
  }
  else if (uid == "73756997")
  {
    result = "S. Gholami";
  }
  else
  {
    result = uid;
  }

  return result;
}

String s(byte *array, int size)
{
  char result[size * 2 + 1];

  memset(result, 0, sizeof(result));

  for (int cnt = 0; cnt < size; cnt++)
  {

    sprintf(&result[cnt * 2], "%02X", array[cnt]);
  }

  return result;
}

void beep()
{

  tone(BEEP_PIN, 700);
  delay(100);
  noTone(BEEP_PIN);
  delay(100);
  tone(BEEP_PIN, 900);
  delay(100);
  noTone(BEEP_PIN);
  delay(100);
}
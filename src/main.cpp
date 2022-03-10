#include <Arduino.h>

#include <RFID.h>
#include <SerialModule.h>
#include <SPI.h>
#include <TimeModule.h>
#include <SDCardModule.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
RFID rfid_module = RFID();
SDCardModule sdcard_module = SDCardModule();
TimeModule time_module = TimeModule();
SerialModule serial = SerialModule();
LiquidCrystal_I2C lcd(0x27, 20, 4);
void initModules();
void updateModules();

String mapIDToName(String uid)
{
  String result;
  if (uid == "396458C2")
  {
    result = "Inosoft00";
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

void setup()
{
  Serial.begin(9600);

  pinMode(SS, OUTPUT);
  pinMode(4, OUTPUT);
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
  rfid_module.init();
  sdcard_module.init();
  lcd.init(); // initialize the lcd
  lcd.backlight();
  // sdcard_module.deleteLog();
  // Time t(2022, 6, 3, 4, 13, 4, Time::kSunday);
  // time_module.init(t);
}
void updateModules()
{
  if (Serial.readString().startsWith("re"))
  {

    sdcard_module.readLog();
  }

  auto cardPresent = rfid_module.update();
  if (cardPresent.size != 0)
  {

    String uid = s(cardPresent.uidByte, cardPresent.size);
    String name = mapIDToName(uid);
    sdcard_module.writeToLog(name + "," + time_module.update());
    lcd.backlight();
    lcd.clear();
    lcd.print(name);
    lcd.setCursor(0, 1);
    lcd.print(time_module.update());
    delay(2000);
  }
  else
  {
    lcd.noBacklight();
    lcd.clear();
    lcd.println(time_module.update());
  }
  delay(250);
}
